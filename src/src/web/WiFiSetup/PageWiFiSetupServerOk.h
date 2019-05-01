#include <Arduino.h>

const char pageWiFiSetupServerOk[] PROGMEM = R"=====(

<!DOCTYPE html>
<head>
  <link
    rel="stylesheet"
    href="/bootstrap.min.css"
  />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta charset="utf-8" />
</head>

<body>
  <div class="container">
    <div class="page-header mt-4">
      <h1>WiFi setup</h1>
      <hr />
    </div>
    <div class="row">
      <div class="col-sm-12 alert alert-success">
        WiFi settings updated! Connecting to WiFi network now.
      </div>
    </div>
</body>


)=====";
