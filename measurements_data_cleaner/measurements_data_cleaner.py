import config
import sys
from datetime import datetime

from postgresdb import PostgresDB
from solarmeasurement_model import SolarMeasurementModel


def get_source_destination_table():
    args = sys.argv
    tables = {'source': args[1], 'destination': args[2]}
    return tables


def get_all_solarmeasurementsmodels_from_table(table):
    fields = ['id', 'cast(resistor_voltage as float)', 'cast(opencircuit_voltage as float)',
              'illuminance_lux', 'created_on']
    rows = db.query_all_rows_from_table(fields, table)
    if not rows:
        return []
    measurements = []
    for row in rows:
        measurements.append(SolarMeasurementModel(
            row[0], row[1], row[2], row[3], row[4]))
    return measurements


def create_measurements_table_if_not_exists(tablename):
    sql = ("CREATE TABLE IF NOT EXISTS {} ( "
           "id serial PRIMARY KEY, "
           "resistor_voltage NUMERIC(4,3) NOT NULL, "
           "opencircuit_voltage NUMERIC(4,3) NOT NULL, "
           "illuminance_lux REAL, "
           "created_on TIMESTAMP NOT NULL);").format(tablename)
    db.execute_sql(sql)


def clean_measurement(measurement):
    measurement.resistor_voltage -= 0.142
    measurement.opencircuit_voltage -= 0.142
    if measurement.resistor_voltage == measurement.opencircuit_voltage and measurement.resistor_voltage > 0:
        measurement.resistor_voltage -= 0.001
    return measurement


def insert_missing_models_in_destination(measurements_in_source, destination):
    for measurement in measurements_in_source:
        measurement = clean_measurement(measurement)
        sql = ("INSERT INTO {} VALUES({}, {}, {}, {}, \'{}\') "
               "ON CONFLICT DO NOTHING").format(destination, measurement.id,
                                                measurement.resistor_voltage, measurement.opencircuit_voltage, measurement.illuminance_lux, measurement.created_on)
        db.execute_sql(sql)


dbparams = config.config_postgres()
db = PostgresDB(dbparams['host'], dbparams['port'],
                dbparams['database'], dbparams['user'], dbparams['password'])

tables = get_source_destination_table()
create_measurements_table_if_not_exists(tables['destination'])
measurements_in_source = get_all_solarmeasurementsmodels_from_table(
    tables['source'])
insert_missing_models_in_destination(
    measurements_in_source, tables['destination'])
