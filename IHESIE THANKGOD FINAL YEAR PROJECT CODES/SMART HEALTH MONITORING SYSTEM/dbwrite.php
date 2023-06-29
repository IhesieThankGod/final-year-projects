// Code Written by Rishi Tiwari
// Website:- https://tricksumo.com
// Reference:- https://www.w3schools.com/php/php_mysql_insert.asp
//
//

<?php



    $host = "localhost";		         // host = localhost because database hosted on the same server where PHP files are hosted
    $dbname = "id19003837_thankyproject";              // Database name
    $username = "id19003837_thankyuser";		// Database username
    $password = "x6/]z}<4AgsL=qR";	        // Database password


// Establish connection to MySQL database
$conn = new mysqli($host, $username, $password, $dbname);


// Check if connection established successfully
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

else { echo "Connected to mysql database. "; }

   
// Get date and time variables
    date_default_timezone_set('Africa/Lagos');  // for other timezones, refer:- https://www.php.net/manual/en/timezones.asia.php
    $d = date("Y-m-d");
    $t = date("H:i:s");
    
// If values send by NodeMCU are not empty then insert into MySQL database table

  if(!empty($_POST['sendtemperatureC']) && !empty($_POST['sendtemperatureF']) )
    {
		$temperatureC = $_POST['sendtemperatureC'];
        $temperatureF = $_POST['sendtemperatureF'];
		$myBPM = $_POST['sendmyBPM'];


// Update your tablename here
	        $sql = "INSERT INTO nodemcu_table (temperatureC, temperatureF, myBPM, Date, Time) VALUES ('".$temperatureC."','".$temperatureF."','".$myBPM."', '".$d."', '".$t."')"; 
 


		if ($conn->query($sql) === TRUE) {
		    echo "Values inserted in MySQL database table.";
		} else {
		    echo "Error: " . $sql . "<br>" . $conn->error;
		}
	}


// Close MySQL connection
$conn->close();



?>
