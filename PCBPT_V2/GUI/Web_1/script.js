// Access the SVG container
const draw = SVG().addTo('#schematic').size(500, 500);

// Create a button
const button = draw.rect(120, 40).fill('#f06').radius(8);
const buttonText = draw.text('Click Me').fill('#fff').font({ size: 18 }).center(button.width() / 2, button.height() / 2);

// Add event listener to the button
button.click(function () {
  console.log('Button clicked!');
  // Customize the button's click behavior here
});

// Create a text input
const input = draw
  .rect(200, 30)
  .fill('#fff')
  .stroke({ color: '#000', width: 1 })
  .radius(4)
  .move(20, 100);
const inputText = draw.text('').font({ size: 16 }).move(30, 110);

// Add event listener to the input
input.mouseup(function () {
  const inputValue = prompt('Enter a value:');
  inputText.text(inputValue);
});
