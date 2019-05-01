#include <Arduino.h>

const char pageHaloghtServerRoot[] PROGMEM = R"=====(
<!DOCTYPE html>
<head>
  <link
    rel="stylesheet"
    href="/bootstrap.min.css"
  />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta charset="utf-8" />
  <script>
    var sendSolidColorButton;
    var solidColorInput;

    window.onload = function() {
      sendSolidColorButton = document.getElementById('sendSolidColorButton');
      solidColorInput = document.getElementById('solidColorInput');

      sendSolidColorButton.addEventListener('click', () => {
        sendPost('/setSolidColor', 'color=' + solidColorInput.value);
      });
    }

    function sendPost(url, data)  {
          var req = new XMLHttpRequest();
          req.open('POST', url, true);
          req.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
          req.send(data);
    }
  </script>
</head>

<body>
  <div class="container">
    <div class="page-header mt-4">
      <h1>Haloght</h1>
      <hr />
    </div>
    <div class="row">
      <div class="col-sm-12">
        <div class="border rounded p-4 mb-3">
          <h4>Solid color</h4>
          <form class="form-horizontal" action="javascript:void(0);">
            <div class="form-group">
              <label class="control-label col-sm-12" for="solidColorInput"
                >Color</label
              >
              <div class="col-sm-12">
                <input
                  type="color"
                  class="form-control"
                  id="solidColorInput"
                  name="solidColorInput"
                  style="height: 50px; max-width: 200px"
                />
              </div>
            </div>
            <div class="form-group">
              <div class="col-sm-offset-3 col-sm-9">
                <button class="btn btn-default" id="sendSolidColorButton">Send</button>
              </div>
            </div>
          </form>
        </div>
      </div>
    </div>
    </div>
  </div>
</body>




)=====";
