#include <Arduino.h>

const char pageHaloghtServerSettings[] PROGMEM = R"=====(

<!DOCTYPE html>

<head>
  <link
    rel="stylesheet"
    href="/bootstrap.min.css"
  />

  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta charset="utf-8" />
  <script>
    var submitButton;
    var resetButton;
    var defaultColorInput;
    var hostnameSuffixInput;

    window.onload = function() {
      submitButton = document.getElementById("submitButton");
      resetButton = document.getElementById("resetButton");
      defaultColorInput = document.getElementById("defaultColorInput");
      hostnameSuffixInput = document.getElementById("hostnameSuffixInput");

      registerResetButton();

      var settingsStateReq = new XMLHttpRequest();
      settingsStateReq.open("GET", "/settings", true);
      settingsStateReq.onreadystatechange = function() {
        if (settingsStateReq.readyState == 4) {
          if (settingsStateReq.status == 200) {
            var data = settingsStateReq.responseText;
            var json = JSON.parse(data);

            defaultColorInput.value = json.defaultColor;
            hostnameSuffixInput.value = json.hostnameSuffix;

            registerSubmitButton();
          } else {
            console.log("Error: Could not load settings!");
          }
        }
      };
      settingsStateReq.send(null);
    };

    function registerResetButton() {
      resetButton.addEventListener("click", function() {
        var confirmation = confirm(
          "Are you sure that you want to reset all settings?"
        );

        if (confirmation) {
          var req = new XMLHttpRequest();
          req.open("POST", "/resetSettings", true);
          req.send(null);

          redirectToRoot();
        }
      });
    }

    function registerSubmitButton() {
      submitButton.addEventListener("click", function() {
        var confirmation = confirm(
          "Are you sure that you want to update the settings?"
        );

        if (confirmation) {
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
        }
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
                <button class="btn btn-secondary" id="submitButton">
                  Submit
                </button>
              </div>
              <div class="col-6">
                <button class="btn btn-danger float-right" id="resetButton">
                  Reset
                </button>
              </div>
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>
</body>


)=====";
