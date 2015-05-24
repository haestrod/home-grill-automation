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
var urlStart = window.location.origin + "/temp/get/20";
var startVolt = [];
var startTemp = [];
var startLabel = [];
var lastID = 0;
$.getJSON(urlStart, function(ret) {
  $.each(ret, function(q) {
    startVolt.push(ret[q].voltage);
    startTemp.push(ret[q].temp);
    startLabel.push(ret[q].created_at);
    lastID = ret[q].id;
  })
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
              scaleIntergersOnly: false
          }
      ]
    };

// Reduce the animation steps for demo clarity.
var myLiveChart = new Chart(ctx).Line(startingData);

var refreshTime = 10000;
setInterval(function(){
  var curTime = Math.floor(Date.now() / 1000);
  var reqTime = curTime - (refreshTime / 1000);
  var curID = 0;
  var url = window.location.origin + "/temp/since/" + reqTime;
  $.getJSON(url, function(k) {
      $.each(k, function(q) {
	if (k[q].id > lastID) {
        	myLiveChart.addData([k[q].voltage], k[q].created_at);
        	//newTemp.push(k[q].temp);
        	//labels.push(k[q].created_at);
        	lastID = k[q].id;
	}
	myLiveChart.update();
      });
  });
}, refreshTime);

		</script>
	</body>
</html>

