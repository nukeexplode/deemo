#pragma once
#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <iostream>

#include "Field.h"

class QueryResult
{
    public:
        typedef std::map<uint32_t, std::string> FieldNames;

        QueryResult(MYSQL_RES* result, uint64_t rowCount, uint32_t fieldCount);
        virtual ~QueryResult();

        virtual bool nextRow();

        uint32_t getField_idx(const std::string& name) const
        {
            auto iter = std::find_if(m_field_name.begin(), m_field_name.end(), [&](auto p){ return p.second == name; });

            if (iter != m_field_name.end()) {
                return iter->first;
            }

            assert(0);
            return uint32_t(-1);
        }

        Field* fetch() const { return m_cur_row; }

        const Field & operator [] (int index) const 
        { 
            return m_cur_row[index];
        }

        const Field& operator [] (const std::string &name) const
        {
            return m_cur_row[getField_idx(name)];
        }

        uint32_t getFieldCount() const { return m_field_count; }
        uint64_t getRowCount() const { return m_row_count; }
        const FieldNames& getFieldNames() const {return m_field_name; }

    private:
        enum Field::DataTypes convertNativeType(enum_field_types mysqlType) const;
	public:
        void endQuery();

    protected:
        Field *                     m_cur_row;
        uint32_t                    m_field_count;
        uint64_t                    m_row_count;
        FieldNames                  m_field_name;

		MYSQL_RES*                  m_result;
};