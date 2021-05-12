import psycopg2


class PostgresDB:

    def __init__(self, host, port, database, user, password):
        self.host = host
        self.port = port
        self.database = database
        self.user = user
        self.password = password
        self.conn = None
        self.cur = None

    def __del__(self):
        self.cur.close()
        self.conn.close()

    def __connect(self):
        if self.conn == None:
            self.conn = psycopg2.connect(host=self.host, port=self.port,
                                         dbname=self.database, user=self.user, password=self.password)
        if self.cur == None:
            self.cur = self.conn.cursor()

    def __is_table_existing(self, table):
        self.__connect()
        sql = (
            "SELECT EXISTS(SELECT 1 FROM information_schema.tables  "
            "WHERE table_catalog='{}' AND "
            "table_schema='public' AND "
            "table_name='{}');"
        ).format(
            self.database,
            table
        )
        self.cur.execute(sql)

        is_existing = self.cur.fetchone()[0]

        return is_existing

    def execute_sql(self, sql):
        self.__connect()
        self.cur.execute(sql)
        self.conn.commit()

    def query_all_rows_from_table(self, fields, table):
        self.__connect()
        fields_string = ','.join(fields)
        sql = "SELECT " + fields_string + " from " + table
        self.cur.execute(sql)
        return self.cur.fetchall()
