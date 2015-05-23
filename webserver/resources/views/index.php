<!doctype html>
<html>
	<head>
		<meta charset="utf-8">
		<title>Home Grill Automation</title>
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.4/jquery.min.js"></script>
		<script src="//www.chartjs.org/assets/Chart.js"></script>
	</head>
	<body>
		<canvas id="updating-chart" width="920" height="600"></canvas>
		<script>
var urlStart = "http://192.168.159.166:8000/temp/last";
var startVolt = [];
var startTemp = [];
var startLabel = [];
var lastID = 0;
$.getJSON(urlStart, function(ret) {
  startVolt.push(ret.voltage);
  startTemp.push(ret.temp);
  startLabel.push(ret.created_at);
  lastID = ret.id;
});

var canvas = document.getElementById('updating-chart'),
    ctx = canvas.getContext('2d'),
    startingData = {
      labels: startLabel,
      datasets: [
          {
              label: "Voltage",
              fillColor: "rgba(151,187,205,0.2)",
              strokeColor: "rgba(151,187,205,1)",
              pointColor: "rgba(151,187,205,1)",
              pointStrokeColor: "#fff",
              pointHighlightFill: "#fff",
              pointHighlightStroke: "rgba(151,187,205,1)",
              data: startVolt,
          }
      ]
    };

// Reduce the animation steps for demo clarity.
var myLiveChart = new Chart(ctx).Line(startingData);


setInterval(function(){
  var newVolt = [];
  var newTemp = [];
  var labels = [];
  var curTime = Math.floor(Date.now() / 1000);
  var reqTime = curTime - 30;
  var curID = 0;
  var url = "http://192.168.159.166:8000/temp/since/" + reqTime;
  $.getJSON(url, function(k) {
      $.each(k, function(q) {
        newVolt.push(q.voltage);
        newTemp.push(q.temp);
        labels.push(q.created_at);
        curID = q.id;
      });
  });

  if (curID != lastID) {
    lastID = curID;
    myLiveChart.addData(newVolt, labels);
  }
}, 3000);

		</script>
	</body>
</html>

