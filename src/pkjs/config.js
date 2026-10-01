var cssLines = [
  'html,body{background:#000000;color:#AAAA55;font-family:"JetBrains Mono",ui-monospace,"Roboto Mono","DejaVu Sans Mono",monospace;-webkit-font-smoothing:auto;}',
  'h1,h2,h3,h4,h5,h6{text-transform:none;color:#AAAA55;letter-spacing:0;}',
  'body>form>.component-heading:first-child h1,.component-heading h1{font-size:1.6rem;color:#FFAA55;}',
  '.section{background:transparent;border:1px solid #555555;border-radius:0;box-shadow:none;}',
  '.section>.component-heading:first-child{background:transparent;border-radius:0;border-bottom:1px solid #555555;}',
  '.section>.component:after{background:#555555;}',
  '.label{color:#AAAA55;}',
  '.component-select .value{color:#FFAA55;}',
  '.component-select .value:after{border-top-color:#FFAA55 !important;}',
  '.component-toggle .slide{background:#555555 !important;border-radius:0 !important;}',
  '.component-toggle .marker{background:#555555 !important;border-radius:0 !important;box-shadow:none !important;}',
  '.component-toggle input:checked+.graphic .slide{background:#550000 !important;}',
  '.component-toggle input:checked+.graphic .marker{background:#FFAA55 !important;}',
  'button[type=submit],.component-submit button{background:#AAAA55 !important;color:#000000 !important;border-radius:0 !important;box-shadow:none !important;text-transform:none !important;font-family:inherit;}',
  'strong{color:#FFAA55;}',
  '.description{color:#AAAA55;opacity:0.8;}',
  '.section{margin-top:1rem;}',
  'h1,h2,h3,h4,h5,h6{font-family:inherit !important;}',
  '.component-heading h1{margin:0.6rem 0 0;}'
];

var styleItem = {
  type: "text",
  defaultValue: "<style>" + cssLines.join("") + "</style>"
};

module.exports = [
  styleItem,
  {
    type: "heading",
    size: 1,
    defaultValue: "Entity"
  },
  {
    type: "section",
    items: [
      { type: "heading", defaultValue: "Time" },
      {
        type: "select",
        messageKey: "ClockFormat",
        label: "Clock format",
        defaultValue: "-1",
        serializeValueAs: "integer",
        options: [
          { label: "Same as the watch", value: "-1" },
          { label: "24-hour", value: "0" },
          { label: "12-hour", value: "1" }
        ]
      },
      {
        type: "select",
        messageKey: "HourColor",
        label: "Hour colour",
        defaultValue: "0",
        serializeValueAs: "integer",
        capabilities: ["COLOR"],
        options: [
          { label: "Red", value: "0" },
          { label: "Cream", value: "1" },
          { label: "Gold", value: "2" }
        ]
      }
    ]
  },
  {
    type: "section",
    items: [
      { type: "heading", defaultValue: "Weather" },
      {
        type: "toggle",
        messageKey: "ShowWeather",
        label: "Show the weather",
        defaultValue: true
      },
      {
        type: "toggle",
        messageKey: "Fahrenheit",
        label: "Use Fahrenheit",
        defaultValue: false
      }
    ]
  },
  {
    type: "section",
    items: [
      { type: "heading", defaultValue: "Motion and alerts" },
      {
        type: "toggle",
        messageKey: "Animate",
        label: "Animate the meter",
        defaultValue: true
      },
      {
        type: "select",
        messageKey: "LowBattery",
        label: "Stop animating at",
        defaultValue: "20",
        serializeValueAs: "integer",
        options: [
          { label: "10% battery", value: "10" },
          { label: "20% battery", value: "20" },
          { label: "30% battery", value: "30" },
          { label: "50% battery", value: "50" }
        ]
      },
      {
        type: "toggle",
        messageKey: "VibeOnDisconnect",
        label: "Buzz when the phone disconnects",
        defaultValue: true
      },
      {
        type: "toggle",
        messageKey: "TapSwap",
        label: "Tap to show steps and heart rate",
        defaultValue: true
      }
    ]
  },
  {
    type: "submit",
    defaultValue: "Save"
  }
];
