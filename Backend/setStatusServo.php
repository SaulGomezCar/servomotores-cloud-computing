<?php
include "connect.php";

try {
	$conn = new PDO("mysql:host=$serverName;dbname=$dataBase", $userName, $password);
	// set the PDO error mode to exception
	$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

	$stmt = $conn->prepare("INSERT INTO statusservo (statusX, statusY, statusZ) VALUES (:vx, :vy, :vz)");

	if (isset($_GET["x"]) && isset($_GET["y"]) && isset($_GET["z"]) ) {
		$x = $_GET["x"];
        $y = $_GET["y"];
        $z = $_GET["z"];

		$stmt->bindParam(':vx', $x);
        $stmt->bindParam(':vy', $y);
        $stmt->bindParam(':vz', $z);

		$stmt->execute();

		echo "New record-> SX: " . $x . " - SY: " . $y . " - SZ: " . $z;
	}
} catch (PDOException $e) {
	echo "Connection failed: " . $e->getMessage();
}

$conn = null;
