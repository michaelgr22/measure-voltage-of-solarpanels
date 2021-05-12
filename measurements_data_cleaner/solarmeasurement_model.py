class SolarMeasurementModel:

    def __init__(self, id, resistor_voltage, opencircuit_voltage, illuminance_lux, created_on):
        self.id = id
        self.resistor_voltage = resistor_voltage
        self.opencircuit_voltage = opencircuit_voltage
        self.illuminance_lux = illuminance_lux
        self.created_on = created_on
