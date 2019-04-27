#include <Arduino.h>

const char pageLedSetupServerOk[] PROGMEM = R"=====(

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
    <div class="page-header">
      <h1>LED setup</h1>
      <hr />
    </div>
    <div class="row">
      <div class="col-sm-12 alert alert-success">
        LED settings updated! Restarting now...
      </div>
    </div>
</body>



)=====";
