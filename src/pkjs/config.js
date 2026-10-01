module.exports = [
  {
    type: "heading",
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
