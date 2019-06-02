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
    var sendFireButton;

    window.onload = function() {
      sendSolidColorButton = document.getElementById("sendSolidColorButton");
      sendFireButton = document.getElementById("sendFireButton");
      solidColorInput = document.getElementById("solidColorInput");

      sendSolidColorButton.addEventListener("click", () => {
        sendPost("/setSolidColor", "color=" + solidColorInput.value);
      });

      sendFireButton.addEventListener("click", () => {
        sendPost("/sendFire", "");
      });
    };

    function sendPost(url, data) {
      var req = new XMLHttpRequest();
      req.open("POST", url, true);
      req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
      req.send(data);
    }
  </script>
  <style>
    .imgHeaderSolidColor {
      background-image: url("data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+CjwhRE9DVFlQRSBzdmcgUFVCTElDICItLy9XM0MvL0RURCBTVkcgMS4xLy9FTiIgImh0dHA6Ly93d3cudzMub3JnL0dyYXBoaWNzL1NWRy8xLjEvRFREL3N2ZzExLmR0ZCI+CjxzdmcgdmVyc2lvbj0iMS4xIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiBwcmVzZXJ2ZUFzcGVjdFJhdGlvPSJ4TWlkWU1pZCBtZWV0IiB2aWV3Qm94PSIwIDAgNjQwIDMyMCIgd2lkdGg9IjY0MCIgaGVpZ2h0PSIzMjAiPjxkZWZzPjxwYXRoIGQ9Ik0xMjUuMDEgMzIwTDI0MC4wMSAwTDE5NS4wMSAwTDgwLjAxIDMyMEwxMjUuMDEgMzIwWiIgaWQ9ImcxRm9OQURMaHciPjwvcGF0aD48bGluZWFyR3JhZGllbnQgaWQ9ImdyYWRpZW50YmFOdndBcmJYIiBncmFkaWVudFVuaXRzPSJ1c2VyU3BhY2VPblVzZSIgeDE9IjE2MC4wMSIgeTE9IjAiIHgyPSIxNjAiIHkyPSIzMjAiPjxzdG9wIHN0eWxlPSJzdG9wLWNvbG9yOiAjOTBmZjAwO3N0b3Atb3BhY2l0eTogMSIgb2Zmc2V0PSIwJSI+PC9zdG9wPjxzdG9wIHN0eWxlPSJzdG9wLWNvbG9yOiAjMDEwMDAwO3N0b3Atb3BhY2l0eTogMCIgb2Zmc2V0PSIxMDAlIj48L3N0b3A+PC9saW5lYXJHcmFkaWVudD48cGF0aCBkPSJNMjA1IDMyMEwzMjAgMEwyNzUgMEwxNjAgMzIwTDIwNSAzMjBaIiBpZD0iYjZLTWZ4OWRCQiI+PC9wYXRoPjxsaW5lYXJHcmFkaWVudCBpZD0iZ3JhZGllbnRhNFU0RWo1Q2JrIiBncmFkaWVudFVuaXRzPSJ1c2VyU3BhY2VPblVzZSIgeDE9IjI0MCIgeTE9IjAiIHgyPSIyNDAiIHkyPSIzMjAiPjxzdG9wIHN0eWxlPSJzdG9wLWNvbG9yOiAjMDBmZmM2O3N0b3Atb3BhY2l0eTogMSIgb2Zmc2V0PSIwJSI+PC9zdG9wPjxzdG9wIHN0eWxlPSJzdG9wLWNvbG9yOiAjMDEwMDAwO3N0b3Atb3BhY2l0eTogMCIgb2Zmc2V0PSIxMDAlIj48L3N0b3A+PC9saW5lYXJHcmFkaWVudD48cGF0aCBkPSJNMjg1LjAxIDMyMEw0MDAgMEwzNTUgMEwyNDAuMDEgMzIwTDI4NS4wMSAzMjBaIiBpZD0iYjF0clIyZmdzRiI+PC9wYXRoPjxsaW5lYXJHcmFkaWVudCBpZD0iZ3JhZGllbnRhWTZZNkEwaE0iIGdyYWRpZW50VW5pdHM9InVzZXJTcGFjZU9uVXNlIiB4MT0iMzIwLjAxIiB5MT0iMCIgeDI9IjMyMC4wMSIgeTI9IjMyMCI+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICMwMDQ1ZmY7c3RvcC1vcGFjaXR5OiAxIiBvZmZzZXQ9IjAlIj48L3N0b3A+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICMwMTAwMDA7c3RvcC1vcGFjaXR5OiAwIiBvZmZzZXQ9IjEwMCUiPjwvc3RvcD48L2xpbmVhckdyYWRpZW50PjxwYXRoIGQ9Ik0zNjUgMzIwTDQ4MCAwTDQzNSAwTDMyMCAzMjBMMzY1IDMyMFoiIGlkPSJkMzJKbVgxdTVhIj48L3BhdGg+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkaWVudGI1NFdpcGlydHciIGdyYWRpZW50VW5pdHM9InVzZXJTcGFjZU9uVXNlIiB4MT0iNDAwIiB5MT0iMCIgeDI9IjQwMCIgeTI9IjMyMCI+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICMwMGZmZDg7c3RvcC1vcGFjaXR5OiAxIiBvZmZzZXQ9IjAlIj48L3N0b3A+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICMwMTAwMDA7c3RvcC1vcGFjaXR5OiAwIiBvZmZzZXQ9IjEwMCUiPjwvc3RvcD48L2xpbmVhckdyYWRpZW50PjxwYXRoIGQ9Ik00NDUuMDEgMzIwTDU2MCAwTDUxNSAwTDQwMCAzMjBMNDQ1LjAxIDMyMFoiIGlkPSJiMm14Q2VoMjV3Ij48L3BhdGg+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkaWVudGk1d3I2NEdVSVMiIGdyYWRpZW50VW5pdHM9InVzZXJTcGFjZU9uVXNlIiB4MT0iNDgwIiB5MT0iMCIgeDI9IjQ4MCIgeTI9IjMyMCI+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICNhYmZmMDA7c3RvcC1vcGFjaXR5OiAxIiBvZmZzZXQ9IjAlIj48L3N0b3A+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICMwMTAwMDA7c3RvcC1vcGFjaXR5OiAwIiBvZmZzZXQ9IjEwMCUiPjwvc3RvcD48L2xpbmVhckdyYWRpZW50PjxwYXRoIGQ9Ik01MjUgMzIwTDY0MCAwTDU5NSAwTDQ4MCAzMjBMNTI1IDMyMFoiIGlkPSJqMXQ3NXVIVkh5Ij48L3BhdGg+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkaWVudGYxa2V5UE5rV2YiIGdyYWRpZW50VW5pdHM9InVzZXJTcGFjZU9uVXNlIiB4MT0iNTYwIiB5MT0iMCIgeDI9IjU2MCIgeTI9IjMyMCI+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICNmZmU3MDA7c3RvcC1vcGFjaXR5OiAxIiBvZmZzZXQ9IjAlIj48L3N0b3A+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICMwMTAwMDA7c3RvcC1vcGFjaXR5OiAwIiBvZmZzZXQ9IjEwMCUiPjwvc3RvcD48L2xpbmVhckdyYWRpZW50PjxwYXRoIGQ9Ik02MDUgMzIwTDcyMCAwTDY3NSAwTDU2MCAzMjBMNjA1IDMyMFoiIGlkPSJhNm43R2FTdVVYIj48L3BhdGg+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkaWVudGMyRm95V1B4amQiIGdyYWRpZW50VW5pdHM9InVzZXJTcGFjZU9uVXNlIiB4MT0iNjQwIiB5MT0iMCIgeDI9IjY0MCIgeTI9IjMyMCI+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICNmZmZmZmY7c3RvcC1vcGFjaXR5OiAxIiBvZmZzZXQ9IjAlIj48L3N0b3A+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICNmZjAwMDA7c3RvcC1vcGFjaXR5OiAxIiBvZmZzZXQ9IjAlIj48L3N0b3A+PHN0b3Agc3R5bGU9InN0b3AtY29sb3I6ICMwMTAwMDA7c3RvcC1vcGFjaXR5OiAwIiBvZmZzZXQ9IjEwMCUiPjwvc3RvcD48L2xpbmVhckdyYWRpZW50PjxwYXRoIGQ9Ik0tMzQuOTkgMzIwTDgwLjAxIDBMMzUuMDEgMEwtNzkuOTkgMzIwTC0zNC45OSAzMjBaIiBpZD0iajFscWlVUkVaRyI+PC9wYXRoPjxsaW5lYXJHcmFkaWVudCBpZD0iZ3JhZGllbnRhclZuTWpkMkYiIGdyYWRpZW50VW5pdHM9InVzZXJTcGFjZU9uVXNlIiB4MT0iMC4wMSIgeTE9IjAiIHgyPSIwLjAxIiB5Mj0iMzIwIj48c3RvcCBzdHlsZT0ic3RvcC1jb2xvcjogI2ZmMDAwMDtzdG9wLW9wYWNpdHk6IDEiIG9mZnNldD0iMCUiPjwvc3RvcD48c3RvcCBzdHlsZT0ic3RvcC1jb2xvcjogIzAxMDAwMDtzdG9wLW9wYWNpdHk6IDAiIG9mZnNldD0iMTAwJSI+PC9zdG9wPjwvbGluZWFyR3JhZGllbnQ+PHBhdGggZD0iTTQ1LjAxIDMyMEwxNjAuMDEgMEwxMTUuMDEgMEwwLjAxIDMyMEw0NS4wMSAzMjBaIiBpZD0iYzROYlZ1UTdpIj48L3BhdGg+PGxpbmVhckdyYWRpZW50IGlkPSJncmFkaWVudGE0UzRGZ2NtWjgiIGdyYWRpZW50VW5pdHM9InVzZXJTcGFjZU9uVXNlIiB4MT0iODAuMDEiIHkxPSIwIiB4Mj0iODAuMDEiIHkyPSIzMjAiPjxzdG9wIHN0eWxlPSJzdG9wLWNvbG9yOiAjZmZkZTAwO3N0b3Atb3BhY2l0eTogMSIgb2Zmc2V0PSIwJSI+PC9zdG9wPjxzdG9wIHN0eWxlPSJzdG9wLWNvbG9yOiAjMDAwMDAwO3N0b3Atb3BhY2l0eTogMCIgb2Zmc2V0PSIxMDAlIj48L3N0b3A+PC9saW5lYXJHcmFkaWVudD48L2RlZnM+PGc+PGc+PGc+PHVzZSB4bGluazpocmVmPSIjZzFGb05BRExodyIgb3BhY2l0eT0iMSIgZmlsbD0idXJsKCNncmFkaWVudGJhTnZ3QXJiWCkiPjwvdXNlPjxnPjx1c2UgeGxpbms6aHJlZj0iI2cxRm9OQURMaHciIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjMDAwMDAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjxnPjx1c2UgeGxpbms6aHJlZj0iI2I2S01meDlkQkIiIG9wYWNpdHk9IjEiIGZpbGw9InVybCgjZ3JhZGllbnRhNFU0RWo1Q2JrKSI+PC91c2U+PGc+PHVzZSB4bGluazpocmVmPSIjYjZLTWZ4OWRCQiIgb3BhY2l0eT0iMSIgZmlsbC1vcGFjaXR5PSIwIiBzdHJva2U9IiMwMDAwMDAiIHN0cm9rZS13aWR0aD0iMSIgc3Ryb2tlLW9wYWNpdHk9IjAiPjwvdXNlPjwvZz48L2c+PGc+PHVzZSB4bGluazpocmVmPSIjYjF0clIyZmdzRiIgb3BhY2l0eT0iMSIgZmlsbD0idXJsKCNncmFkaWVudGFZNlk2QTBoTSkiPjwvdXNlPjxnPjx1c2UgeGxpbms6aHJlZj0iI2IxdHJSMmZnc0YiIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjMDAwMDAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjxnPjx1c2UgeGxpbms6aHJlZj0iI2QzMkptWDF1NWEiIG9wYWNpdHk9IjEiIGZpbGw9InVybCgjZ3JhZGllbnRiNTRXaXBpcnR3KSI+PC91c2U+PGc+PHVzZSB4bGluazpocmVmPSIjZDMySm1YMXU1YSIgb3BhY2l0eT0iMSIgZmlsbC1vcGFjaXR5PSIwIiBzdHJva2U9IiMwMDAwMDAiIHN0cm9rZS13aWR0aD0iMSIgc3Ryb2tlLW9wYWNpdHk9IjAiPjwvdXNlPjwvZz48L2c+PGc+PHVzZSB4bGluazpocmVmPSIjYjJteENlaDI1dyIgb3BhY2l0eT0iMSIgZmlsbD0idXJsKCNncmFkaWVudGk1d3I2NEdVSVMpIj48L3VzZT48Zz48dXNlIHhsaW5rOmhyZWY9IiNiMm14Q2VoMjV3IiBvcGFjaXR5PSIxIiBmaWxsLW9wYWNpdHk9IjAiIHN0cm9rZT0iIzAwMDAwMCIgc3Ryb2tlLXdpZHRoPSIxIiBzdHJva2Utb3BhY2l0eT0iMCI+PC91c2U+PC9nPjwvZz48Zz48dXNlIHhsaW5rOmhyZWY9IiNqMXQ3NXVIVkh5IiBvcGFjaXR5PSIxIiBmaWxsPSJ1cmwoI2dyYWRpZW50ZjFrZXlQTmtXZikiPjwvdXNlPjxnPjx1c2UgeGxpbms6aHJlZj0iI2oxdDc1dUhWSHkiIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjMDAwMDAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjxnPjx1c2UgeGxpbms6aHJlZj0iI2E2bjdHYVN1VVgiIG9wYWNpdHk9IjEiIGZpbGw9InVybCgjZ3JhZGllbnRjMkZveVdQeGpkKSI+PC91c2U+PGc+PHVzZSB4bGluazpocmVmPSIjYTZuN0dhU3VVWCIgb3BhY2l0eT0iMSIgZmlsbC1vcGFjaXR5PSIwIiBzdHJva2U9IiMwMDAwMDAiIHN0cm9rZS13aWR0aD0iMSIgc3Ryb2tlLW9wYWNpdHk9IjAiPjwvdXNlPjwvZz48L2c+PGc+PHVzZSB4bGluazpocmVmPSIjajFscWlVUkVaRyIgb3BhY2l0eT0iMSIgZmlsbD0idXJsKCNncmFkaWVudGFyVm5NamQyRikiPjwvdXNlPjxnPjx1c2UgeGxpbms6aHJlZj0iI2oxbHFpVVJFWkciIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjMDAwMDAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjxnPjx1c2UgeGxpbms6aHJlZj0iI2M0TmJWdVE3aSIgb3BhY2l0eT0iMSIgZmlsbD0idXJsKCNncmFkaWVudGE0UzRGZ2NtWjgpIj48L3VzZT48Zz48dXNlIHhsaW5rOmhyZWY9IiNjNE5iVnVRN2kiIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjMDAwMDAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjwvZz48L2c+PC9zdmc+");
      height: 80px;
      background-size: contain;
      background-origin: content-box;
    }

    .imgHeaderFire {
      background-image: url("data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+CjwhRE9DVFlQRSBzdmcgUFVCTElDICItLy9XM0MvL0RURCBTVkcgMS4xLy9FTiIgImh0dHA6Ly93d3cudzMub3JnL0dyYXBoaWNzL1NWRy8xLjEvRFREL3N2ZzExLmR0ZCI+CjxzdmcgdmVyc2lvbj0iMS4xIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiBwcmVzZXJ2ZUFzcGVjdFJhdGlvPSJ4TWlkWU1pZCBtZWV0IiB2aWV3Qm94PSIwIDAgNjQwIDMyMCIgd2lkdGg9IjY0MCIgaGVpZ2h0PSIzMjAiPjxkZWZzPjxwYXRoIGQ9Ik0zODcuMDIgMzIwQzQwMy4yIDI3NS41NCAzOTUuNjIgMjQzLjk4IDM2NC4yOSAyMjUuMzJDMzMyLjk2IDIwNi42NSAzMDkuNzMgMTU2LjQgMjk0LjU5IDc0LjU3QzIzMy41NCAxNzMuMjkgMjE5LjY5IDI1NS4xIDI1My4wMiAzMjAiIGlkPSJiYlFHQW5oSHoiPjwvcGF0aD48cGF0aCBkPSJNMTEyLjEyIDMyMEM2OS42OCAyNzUuMDUgNjQuMTIgMjQzLjI0IDk1LjQ1IDIyNC41N0MxNDIuNDUgMTk2LjU3IDgxLjk4IDIwLjEyIDgwLjk4IDE1LjQ4QzI1MS45OCA4OC44NCAyOTYuMTIgMjIyLjY0IDI0Ni4xMiAzMjAiIGlkPSJjMjFETXRub0xZIj48L3BhdGg+PHBhdGggZD0iTTE2NS4xMiAzMjBDMTM5LjQ1IDI4MS44OSAxMzQuNzkgMjU5LjcyIDE1MS4xMiAyNTMuNDhDMTkwLjYyIDIwMy4xMiAxNTMuMTIgMTMxLjI3IDE1Ni4xMiAxMzAuNjNDMjE4Ljc5IDE3OC44NyAyMzguNDUgMjQxLjk5IDIxNS4xMiAzMjAiIGlkPSJoMnhYNTMwQ0U5Ij48L3BhdGg+PHBhdGggZD0iTTE1OS42MSAzMjBDMTkwLjk0IDI3NS4wNSAxOTAuOTQgMjQzLjI0IDE1OS42MSAyMjQuNTdDMTEyLjYxIDE5Ni41NyAxNjMuOTkgOTIuODUgMTY0Ljk5IDg4LjIxQy02LjAxIDE2MS41NiAtMjYuNDggMjIyLjY0IDIzLjUyIDMyMCIgaWQ9ImUxNXN2WVpJbVMiPjwvcGF0aD48cGF0aCBkPSJNMzM0LjAyIDMyMEMzNTAuMzUgMjg3Ljk1IDM1MC4zNSAyNjguODEgMzM0LjAyIDI2Mi41N0MyOTQuNTIgMjEyLjIxIDI5Ni45IDE5NS44MSAyODguNDcgMTg1LjkyQzI2Mi42NiAyMjYuODEgMjYxLjE3IDI3MS41IDI4NC4wMiAzMjAiIGlkPSJibG5xY21WQ0QiPjwvcGF0aD48cGF0aCBkPSJNMTA2LjI2IDMyMEMxMzEuOTMgMjg3Ljk1IDEzNi41OSAyNjguODEgMTIwLjI2IDI2Mi41N0M4MC43NiAyMTIuMjEgMTE4LjI2IDE3NS4yMSAxMTUuMjYgMTc0LjU3QzUyLjU5IDIyMi44MSAzMi45MyAyNzEuMjkgNTYuMjYgMzIwIiBpZD0iYTlVQjZuQWFUIj48L3BhdGg+PHBhdGggZD0iTTUwMC45NiAzMjBDNTI2LjIzIDI2Ny40NiA1MjMuMiAyMzEuODYgNDkxLjg3IDIxMy4xOUM0NDQuODcgMTg1LjE5IDQ4Ny44NyAxMDkuOTkgNDgyLjc4IDEwNC44N0MzOTguOSAxMzAuMDYgMzE2Ljk2IDIyMi42NCAzNjYuOTYgMzIwIiBpZD0ia0tNQzE2Qnd5Ij48L3BhdGg+PHBhdGggZD0iTTQ0OS4yNyAzMjBDNDcyLjcgMjk2LjY2IDQ3Ni4yNCAyODEuODcgNDU5LjkxIDI3NS42M0M0MjAuNDEgMjI1LjI3IDQ0My4yMiAyMDIuNDcgNDM0LjggMTkyLjU5QzQwOC45OCAyMzMuNDcgMzk3LjE0IDI3NS45NCAzOTkuMjcgMzIwIiBpZD0iZTFlbHJ1TU5DWCI+PC9wYXRoPjxwYXRoIGQ9Ik00OTcuNjMgMzIwQzQ3MS41OCAyMzEuMTIgNTY5LjY2IDE3MS4yNSA1NDguOTkgNjQuNUM1MzcuMzUgNTMuNTcgNjE0LjQ0IDEyMy41MyA2MjguODUgMTg3LjQxQzY0My4yNiAyNTEuMjkgNjQ1LjA2IDI4Ni40NyA2MjYuNzcgMzIyLjA4IiBpZD0iY05aYzVmMlNmIj48L3BhdGg+PHBhdGggZD0iTTU0OC4zNiAzMjBDNTU2LjU2IDI4NC4yMSA0ODguMTcgMjQ3LjYzIDU4MS4xNiAxNzYuODJDNTY2Ljc2IDI2Ni43MSA2MTYuOTggMjM5Ljc2IDU5OC4zNiAzMjAiIGlkPSJhMklwTEZ0TXNtIj48L3BhdGg+PC9kZWZzPjxnPjxnPjxnPjx1c2UgeGxpbms6aHJlZj0iI2JiUUdBbmhIeiIgb3BhY2l0eT0iMSIgZmlsbD0iI2ZmNWUyYSIgZmlsbC1vcGFjaXR5PSIxIj48L3VzZT48Zz48dXNlIHhsaW5rOmhyZWY9IiNiYlFHQW5oSHoiIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjMDAwMDAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjxnPjx1c2UgeGxpbms6aHJlZj0iI2MyMURNdG5vTFkiIG9wYWNpdHk9IjEiIGZpbGw9IiNjNjA1MDAiIGZpbGwtb3BhY2l0eT0iMSI+PC91c2U+PGc+PHVzZSB4bGluazpocmVmPSIjYzIxRE10bm9MWSIgb3BhY2l0eT0iMSIgZmlsbC1vcGFjaXR5PSIwIiBzdHJva2U9IiMwMDAwMDAiIHN0cm9rZS13aWR0aD0iMSIgc3Ryb2tlLW9wYWNpdHk9IjAiPjwvdXNlPjwvZz48L2c+PGc+PHVzZSB4bGluazpocmVmPSIjaDJ4WDUzMENFOSIgb3BhY2l0eT0iMSIgZmlsbD0iI2ZmYjIwMCIgZmlsbC1vcGFjaXR5PSIxIj48L3VzZT48Zz48dXNlIHhsaW5rOmhyZWY9IiNoMnhYNTMwQ0U5IiBvcGFjaXR5PSIxIiBmaWxsLW9wYWNpdHk9IjAiIHN0cm9rZT0iI2ZmYTgwMCIgc3Ryb2tlLXdpZHRoPSIxIiBzdHJva2Utb3BhY2l0eT0iMCI+PC91c2U+PC9nPjwvZz48Zz48dXNlIHhsaW5rOmhyZWY9IiNlMTVzdllaSW1TIiBvcGFjaXR5PSIxIiBmaWxsPSIjZmY3ZDU0IiBmaWxsLW9wYWNpdHk9IjEiPjwvdXNlPjxnPjx1c2UgeGxpbms6aHJlZj0iI2UxNXN2WVpJbVMiIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjMDAwMDAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjxnPjx1c2UgeGxpbms6aHJlZj0iI2JsbnFjbVZDRCIgb3BhY2l0eT0iMSIgZmlsbD0iI2ZmYzMwMCIgZmlsbC1vcGFjaXR5PSIxIj48L3VzZT48Zz48dXNlIHhsaW5rOmhyZWY9IiNibG5xY21WQ0QiIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjZmZhODAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjxnPjx1c2UgeGxpbms6aHJlZj0iI2E5VUI2bkFhVCIgb3BhY2l0eT0iMSIgZmlsbD0iI2ZmZGUwMCIgZmlsbC1vcGFjaXR5PSIxIj48L3VzZT48Zz48dXNlIHhsaW5rOmhyZWY9IiNhOVVCNm5BYVQiIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjZmZhODAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjxnPjx1c2UgeGxpbms6aHJlZj0iI2tLTUMxNkJ3eSIgb3BhY2l0eT0iMSIgZmlsbD0iI2ZmYTk3ZCIgZmlsbC1vcGFjaXR5PSIxIj48L3VzZT48Zz48dXNlIHhsaW5rOmhyZWY9IiNrS01DMTZCd3kiIG9wYWNpdHk9IjEiIGZpbGwtb3BhY2l0eT0iMCIgc3Ryb2tlPSIjMDAwMDAwIiBzdHJva2Utd2lkdGg9IjEiIHN0cm9rZS1vcGFjaXR5PSIwIj48L3VzZT48L2c+PC9nPjxnPjx1c2UgeGxpbms6aHJlZj0iI2UxZWxydU1OQ1giIG9wYWNpdHk9IjEiIGZpbGw9IiNmZmQ1MDAiIGZpbGwtb3BhY2l0eT0iMSI+PC91c2U+PGc+PHVzZSB4bGluazpocmVmPSIjZTFlbHJ1TU5DWCIgb3BhY2l0eT0iMSIgZmlsbC1vcGFjaXR5PSIwIiBzdHJva2U9IiNmZmE4MDAiIHN0cm9rZS13aWR0aD0iMSIgc3Ryb2tlLW9wYWNpdHk9IjAiPjwvdXNlPjwvZz48L2c+PGc+PHVzZSB4bGluazpocmVmPSIjY05aYzVmMlNmIiBvcGFjaXR5PSIxIiBmaWxsPSIjZmY1ODM4IiBmaWxsLW9wYWNpdHk9IjEiPjwvdXNlPjxnPjx1c2UgeGxpbms6aHJlZj0iI2NOWmM1ZjJTZiIgb3BhY2l0eT0iMSIgZmlsbC1vcGFjaXR5PSIwIiBzdHJva2U9IiMwMDAwMDAiIHN0cm9rZS13aWR0aD0iMSIgc3Ryb2tlLW9wYWNpdHk9IjAiPjwvdXNlPjwvZz48L2c+PGc+PHVzZSB4bGluazpocmVmPSIjYTJJcExGdE1zbSIgb3BhY2l0eT0iMSIgZmlsbD0iI2ZmYzQwMCIgZmlsbC1vcGFjaXR5PSIxIj48L3VzZT48Zz48dXNlIHhsaW5rOmhyZWY9IiNhMklwTEZ0TXNtIiBvcGFjaXR5PSIxIiBmaWxsLW9wYWNpdHk9IjAiIHN0cm9rZT0iI2ZmYTgwMCIgc3Ryb2tlLXdpZHRoPSIxIiBzdHJva2Utb3BhY2l0eT0iMCI+PC91c2U+PC9nPjwvZz48L2c+PC9nPjwvc3ZnPg==");
      height: 80px;
      background-size: contain;
      background-origin: content-box;
    }
  </style>
</head>

<body>
  <div class="container">
    <div class="page-header mt-4">
      <h1>Haloght</h1>
      <hr />
    </div>
    <div class="row">
      <div class="col-sm-6 pb-3">
        <div class="card">
          <div
            class="card-img-top pl-2 pr-2 pt-1"
            style="background-color: #F3F3F3;"
          >
            <div class="imgHeaderSolidColor"></div>
          </div>
          <div class="card-body">
            <h4>Solid color</h4>
            <form class="form-horizontal" action="javascript:void(0);">
              <div class="form-group">
                <div class="col-sm-12">
                  <input
                    type="color"
                    class="form-control"
                    id="solidColorInput"
                    name="solidColorInput"
                    style="height: 50px; width: 100%;"
                  />
                </div>
              </div>
              <div class="form-group">
                <div class="col-sm-12">
                  <button
                    class="btn btn-default"
                    style="width: 100%;"
                    id="sendSolidColorButton"
                  >
                    Send
                  </button>
                </div>
              </div>
            </form>
          </div>
        </div>
      </div>
      <div class="col-sm-6 pb-3">
        <div class="card">
          <div
            class="card-img-top pl-2 pr-2 pt-1"
            style="background-color: #F3F3F3;"
          >
            <div class="imgHeaderFire"></div>
          </div>
          <div class="card-body">
            <h4>Fire</h4>
            <form class="form-horizontal" action="javascript:void(0);">
              <div class="form-group">
                <div class="col-sm-12">
                  <button
                    class="btn btn-default"
                    style="width: 100%;"
                    id="sendFireButton"
                  >
                    Send
                  </button>
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
