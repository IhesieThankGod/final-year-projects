// Code Written by Rishi Tiwari
// Website:- https://tricksumo.com
// Reference:- https://www.w3schools.com/php/php_mysql_select.asp
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

else { echo "Connected to mysql database. <br>"; }


// Select values from MySQL database table

$sql = "SELECT id, temperatureC, temperatureF, myBPM, date, time FROM nodemcu_table";  // Update your tablename here

$result = $conn->query($sql);

echo "<center>";



if ($result->num_rows > 0) {


    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<strong> Id:</strong> " . $row["id"]. " &nbsp <strong>temperatureC:</strong> " . $row["temperatureC"]. " &nbsp <strong>temperatureF:</strong> " . $row["temperatureF"]. " &nbsp <strong>myBPM:</strong>". $row["myBPM"]. " &nbsp <strong>Date:</strong> " . $row["date"]." &nbsp <strong>Time:</strong>" .$row["time"]. "<p>";
    


}
} else {
    echo "0 results";
}

echo "</center>";

$conn->close();



?>
