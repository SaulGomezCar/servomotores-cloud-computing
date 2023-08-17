<?php
include "connect.php";
header('Access-Control-Allow-Origin: *');
header('Content-Type: application/json; charset=utf-8');

try {
    $conn = new PDO("mysql:host=$serverName;dbname=$dataBase", $userName, $password);
    // set the PDO error mode to exception
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $sql = 'SELECT reg_date, statusX, statusY, statusZ FROM statusservo ORDER BY reg_date DESC LIMIT 10';
    
    $results = array();

    foreach ($conn->query($sql) as $row) {
        $result = array(
            'reg_date' => $row['reg_date'],
            'statusX' => intval($row['statusX']),
            'statusY' => intval($row['statusY']),
            'statusZ' => intval($row['statusZ'])
        );
        $results[] = $result;
    }

    echo json_encode($results);
} catch (PDOException $e) {
    echo "Connection failed: " . $e->getMessage();
}

$conn = null;
