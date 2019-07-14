#include <Arduino.h>

const char pageHaloghtServerSettings[] PROGMEM = R"=====(

<!DOCTYPE html>

<head>
  <link
    rel="stylesheet"
    href="/bootstrap.min.css"
  />
  <script
    src="/jquery-3.3.1.slim.min.js"
    integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo"
    crossorigin="anonymous"
  ></script>
  <script
    src="/popper.min.js"
    integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1"
    crossorigin="anonymous"
  ></script>
  <script
    src="/bootstrap.min.js"
    integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM"
    crossorigin="anonymous"
  ></script>

  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta charset="utf-8" />
  <script>
    var submitConfirmButton;
    var resetConfirmButton;
    var defaultColorInput;
    var hostnameSuffixInput;

    window.onload = function() {
      submitConfirmButton = document.getElementById("submitConfirmButton");
      resetConfirmButton = document.getElementById("resetConfirmButton");
      defaultColorInput = document.getElementById("defaultColorInput");
      hostnameSuffixInput = document.getElementById("hostnameSuffixInput");

      var settingsStateReq = new XMLHttpRequest();
      settingsStateReq.open("GET", "/settings", true);
      settingsStateReq.onreadystatechange = function() {
        if (settingsStateReq.readyState == 4) {
          if (settingsStateReq.status == 200) {
            var data = settingsStateReq.responseText;
            var json = JSON.parse(data);

            defaultColorInput.value = json.defaultColor;
            hostnameSuffixInput.value = json.hostnameSuffix;

            registerButtons();
          } else {
            console.log("Error: Could not load settings!");
          }
        }
      };
      settingsStateReq.send(null);
    };

    function registerButtons() {
      submitConfirmButton.addEventListener("click", function() {
        var json = {};
        json.defaultColor = defaultColorInput.value;
        json.hostnameSuffix = hostnameSuffixInput.value;

        var data = JSON.stringify(json);

        var updateSettingsReq = new XMLHttpRequest();
        updateSettingsReq.open("POST", "/settings", true);
        updateSettingsReq.onreadystatechange = function() {
          if (updateSettingsReq.readyState == 4) {
            if (updateSettingsReq.status == 200) {
              redirectToRoot();
            } else {
              console.log("Error: Could not update settings!");
            }
          }
        };
        updateSettingsReq.send(data);
      });

      resetConfirmButton.addEventListener("click", function() {
        var req = new XMLHttpRequest();
        req.open("POST", "/resetSettings", true);
        req.send(null);

        redirectToRoot();
      });
    }

    function redirectToRoot() {
      setTimeout(function() {
        window.location.href = "/";
      }, 3000);
    }
  </script>
  <style></style>
</head>

<body>
  <div class="container">
    <div class="page-header mt-4">
      <div class="row h-100">
        <div class="col-sm-7">
          <h1 class="display-3">Haloght</h1>
        </div>
      </div>
      <hr />
    </div>
    <div class="row">
      <div class="col-sm-8">
        <form class="form-horizontal" action="javascript:void(0);">
          <div class="form-group">
            <label class="control-label" for="defaultColorInput"
              >Default color</label
            >
            <input
              type="color"
              class="form-control"
              id="defaultColorInput"
              name="defaultColorInput"
              style="height: 50px;"
            />
          </div>
          <div class="form-group">
            <label class="control-label" for="numLeds">Hostname</label>
            <div class="row">
              <div class="col-4 my-auto pr-0">
                <label
                  for="hostnameSuffixInput"
                  class="float-right"
                  style="padding-right: 0px;"
                  >Haloght -&nbsp;</label
                >
              </div>
              <div class="col-8 pl-0">
                <input
                  type="text"
                  class="form-control float-left"
                  id="hostnameSuffixInput"
                  name="hostnameSuffixInput"
                  placeholder="Suffix..."
                />
              </div>
            </div>
          </div>
          <div class="form-group">
            <div class="row">
              <div class="col-6">
                <button
                  class="btn btn-secondary"
                  data-target="#submitButtonModal"
                  data-toggle="modal"
                >
                  Submit
                </button>
              </div>
              <div class="col-6">
                <button
                  class="btn btn-danger float-right"
                  data-target="#resetButtonModal"
                  data-toggle="modal"
                >
                  Reset
                </button>
              </div>
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>
  <div class="modal fade" id="resetButtonModal" tabindex="-1" role="dialog">
    <div class="modal-dialog" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title">Reset</h5>
          <button
            type="button"
            class="close"
            data-dismiss="modal"
            aria-label="Close"
          >
            <span aria-hidden="true">&times;</span>
          </button>
        </div>
        <div class="modal-body">
          <p>Are you sure you want to reset all settings?</p>
        </div>
        <div class="modal-footer">
          <button type="button" class="btn btn-primary" id="resetConfirmButton">
            Yes
          </button>
          <button type="button" class="btn btn-secondary" data-dismiss="modal">
            No
          </button>
        </div>
      </div>
    </div>
  </div>
  <div class="modal fade" id="submitButtonModal" tabindex="-1" role="dialog">
    <div class="modal-dialog" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title">Submit changes</h5>
          <button
            type="button"
            class="close"
            data-dismiss="modal"
            aria-label="Close"
          >
            <span aria-hidden="true">&times;</span>
          </button>
        </div>
        <div class="modal-body">
          <p>
            Are you sure you want to submit the changes? The microcontroller
            will restart afterwards.
          </p>
        </div>
        <div class="modal-footer">
          <button
            type="button"
            class="btn btn-primary"
            id="submitConfirmButton"
          >
            Yes
          </button>
          <button type="button" class="btn btn-secondary" data-dismiss="modal">
            No
          </button>
        </div>
      </div>
    </div>
  </div>
</body>


)=====";
