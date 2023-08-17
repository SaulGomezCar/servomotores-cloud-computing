<?php
include "connect.php";
header('Access-Control-Allow-Origin: *');
header('Content-Type: application/json; charset=utf-8');

try {
    $conn = new PDO("mysql:host=$serverName;dbname=$dataBase", $userName, $password);
    // set the PDO error mode to exception
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $sql = 'SELECT reg_date, statusX, statusY, statusZ FROM statusgiroscopio ORDER BY reg_date DESC LIMIT 1';
    
    $results = array();

    foreach ($conn->query($sql) as $row) {
        $result = array(
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
