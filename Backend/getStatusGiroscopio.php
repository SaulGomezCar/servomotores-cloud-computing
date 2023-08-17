<?php
include "connect.php";
header('Access-Control-Allow-Origin: *');
header('Content-Type: application/json');

try {
    $conn = new PDO("mysql:host=$serverName;dbname=$dataBase", $userName, $password);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $sql = 'SELECT reg_date, statusX, statusY, statusZ FROM statusgiroscopio ORDER BY reg_date DESC LIMIT 10';

    $results = array();

    foreach ($conn->query($sql) as $row) {
        $result = array(
            'reg_date' => $row['reg_date'],
            'statusX' => floatval($row['statusX']),
            'statusY' => floatval($row['statusY']),
            'statusZ' => floatval($row['statusZ'])
        );
        $results[] = $result;
    }

    echo json_encode($results);
} catch (PDOException $e) {
    echo "Connection failed: " . $e->getMessage();
}

$conn = null;