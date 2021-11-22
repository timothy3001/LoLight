#include <Arduino.h>

const char pageLedSetupServerRoot[] PROGMEM = R"=====(
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
      <h1>LED setup</h1>
      <hr />
    </div>
    <div class="row">
      <div class="col-sm-8">
        <form class="form-horizontal" action="/config" method="post">
          <div class="form-group">
            <label class="control-label col-sm-5" for="dataPin"
              >LED data pin</label
            >
            <div class="col-sm-7">
              <input
                type="number"
                class="form-control"
                id="dataPin"
                name="dataPin"
                min="0"
                max="50"
                placeholder="Enter LED data pin..."
              />
            </div>
          </div>
          <div class="form-group">
            <label class="control-label col-sm-5" for="numLeds"
              >Number of LEDs</label
            >
            <div class="col-sm-7">
              <input
                type="number"
                class="form-control"
                id="numLeds"
                name="numLeds"
                min="1"
                max="1000"
                placeholder="Enter number of LEDs..."
              />
            </div>
          </div>
          <div class="form-group">
            <label class="control-label col-sm-5" for="ledType"
              >LED Type</label
            >
            <div class="col-sm-7">
              <select
                class="form-control"
                id="ledType"
                name="ledType"
              >
                <option value="0">SK6812 RGBW</option>
                <option value="1">WS2815</option>
              </select>
            </div>
          </div>
          <div class="form-group">
            <div class="col-sm-offset-3 col-sm-9">
              <button type="submit" class="btn btn-default">Submit</button>
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>
</body>

)=====";
