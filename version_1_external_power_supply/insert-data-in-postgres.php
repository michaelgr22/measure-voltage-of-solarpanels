<?php


$servername = "10.1.43.22";
$port = "5432";

// REPLACE with your Database name
$dbname = "arduinodb";
// REPLACE with Database user
$username = "esp8266";
// REPLACE with Database user password
$password = "Esp8266_";

$voltage = $measure_time = "";

echo "Hello World";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $voltage = test_input($_POST["voltage"]);
        $measure_time = test_input($_POST["measure_time"]);

	$conn_string = "host=" . $servername . " port=" . $port . " dbname=" . $dbname . " user=" . $username . " password=" . $password . "";
	$conn = pg_connect($conn_string);

        if (!$conn) {
	  echo "Ein Fehler ist aufgetreten.\n";
	  exit;
	}

        $sql = "INSERT INTO solar_data (voltage, measure_time) VALUES ('" . $voltage . "', '" . $measure_time . "')";

        $result = pg_query($conn, $sql);
	if (!$result) {
	  echo "Ein Fehler ist aufgetreten.\n";
	  exit;
	}

    pg_close($conn);

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>
