#include "QueryResult.h"

QueryResult::QueryResult(MYSQL_RES* result, uint64_t rowCount, uint32_t fieldCount) {
    m_result = result;
    m_row_count = rowCount;
    m_field_count = fieldCount;

    m_cur_row = new Field[m_field_count];

    MYSQL_FIELD* fields = mysql_fetch_fields(m_result);

    std::cout << m_cur_row << std::endl;
    for (uint32_t i = 0; i < fieldCount; i++) {
        m_field_name[i] = fields[i].name;
        m_cur_row[i].setName(fields[i].name);
        m_cur_row[i].setType(convertNativeType(fields[i].type));
    }
}

QueryResult::~QueryResult(void)
{
    std::cout << m_cur_row << std::endl;
    if (m_cur_row != nullptr) {
        delete [] m_cur_row;
    }

    if (m_result != nullptr) {
        mysql_free_result(m_result);
    }
}

bool QueryResult::nextRow()
{
    MYSQL_ROW row;

    assert(m_result != nullptr);

    row = mysql_fetch_row(m_result);
    if (!row)
    {
        return false;
    }

    unsigned long* field_length;
    field_length = mysql_fetch_lengths(m_result);
    for (uint32_t i = 0; i < m_field_count; i++)
    {
        if(row[i] == NULL)
        {
            m_cur_row[i].m_bNULL = true;
            m_cur_row[i].setValue("", 0);
        }
        else
        {
            m_cur_row[i].m_bNULL = false;
            m_cur_row[i].setValue(row[i], field_length[i]);
        }
    }

    return true;
}

enum Field::DataTypes QueryResult::convertNativeType(enum_field_types mysqlType) const
{
    switch (mysqlType)
    {
        case FIELD_TYPE_TIMESTAMP:
        case FIELD_TYPE_DATE:
        case FIELD_TYPE_TIME:
        case FIELD_TYPE_DATETIME:
        case FIELD_TYPE_YEAR:
        case FIELD_TYPE_STRING:
        case FIELD_TYPE_VAR_STRING:
        case FIELD_TYPE_BLOB:
        case FIELD_TYPE_SET:
        case FIELD_TYPE_NULL:
            return Field::DB_TYPE_STRING;
        case FIELD_TYPE_TINY:

        case FIELD_TYPE_SHORT:
        case FIELD_TYPE_LONG:
        case FIELD_TYPE_INT24:
        case FIELD_TYPE_LONGLONG:
        case FIELD_TYPE_ENUM:
            return Field::DB_TYPE_INTEGER;
        case FIELD_TYPE_DECIMAL:
        case FIELD_TYPE_FLOAT:
        case FIELD_TYPE_DOUBLE:
            return Field::DB_TYPE_FLOAT;
        default:
            return Field::DB_TYPE_UNKNOWN;
    }
}