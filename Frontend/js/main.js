google.charts.load('current', { packages: ['corechart', 'line'] });
google.charts.setOnLoadCallback(drawCharts);

function drawCharts() {
  // Gráfica de giroscopio
  axios
    .get('https://app.saulgomezc.site/iot-curso-itp-db/backend/getStatusServo.php')
    .then(function (response) {
      var jsonData = response.data;

      var data = new google.visualization.DataTable();
      data.addColumn('date', 'X');
      data.addColumn('number', 'Servo X');
      data.addColumn('number', 'Servo Y');
      data.addColumn('number', 'Servo Z');

      jsonData.forEach(function (row) {
        data.addRow([new Date(row.reg_date), row.statusX, row.statusY, row.statusZ]);
      });

      var options = {
        hAxis: {
          title: 'Tiempo',
        },
        vAxis: {
          title: 'Valores del Servo Motor',
        },
      };

      var chart = new google.visualization.LineChart(
        document.getElementById('chart_div_servo')
      );

      chart.draw(data, options);
    })
    .catch(function (error) {
      console.error('Error al cargar el JSON de giroscopio:', error);
    });
}

setInterval(drawCharts, 1000);
