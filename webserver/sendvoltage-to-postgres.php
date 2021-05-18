<html>

<head>
    <title>PHP-Test</title>
</head>

<body>
    <?php

    $servername = "10.152.183.120";
    $port = "5432";

    // REPLACE with your Database name
    $dbname = "measurements";
    // REPLACE with Database user
    $username = "esp32";
    // REPLACE with Database user password
    $password = "esp32_123";


    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $resistor_voltage = test_input($_POST["resistorvoltage"]);
        $opencircuit_voltage = test_input($_POST["opencircuitvoltage"]);
        $illuminance_lux = test_input($_POST["illuminancelux"]);

        echo $resistor_voltage;
        echo $opencircuit_voltage;
        $conn_string = "host=" . $servername . " port=" . $port . " dbname=" . $dbname . " user=" . $username . " password=" . $password . "";
        $conn = pg_connect($conn_string);

        if (!$conn) {
                echo "Ein Fehler ist aufgetreten.\n";
                echo "test";
            exit;
        }

        $sql = "INSERT INTO solar_measurement_2_220 (resistor_voltage,opencircuit_voltage,illuminance_lux,created_on)
        VALUES ('" . $resistor_voltage . "', '" . $opencircuit_voltage . "', '" . $illuminance_lux . "', current_timestamp)";

        $result = pg_query($conn, $sql);
        if (!$result) {
            echo "Ein Fehler ist aufgetreten.\n";
            exit;
        }

        pg_close($conn);
    }
    function test_input($data)
    {
        $data = trim($data);
        $data = stripslashes($data);
        $data = htmlspecialchars($data);
        return $data;
    }
    ?>
</body>

</html>
