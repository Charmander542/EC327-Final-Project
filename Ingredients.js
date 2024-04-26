//const {ipcRenderer} = require('electron');
//const fs = require("fs");

//expire();
//get current ingredient information
document.addEventListener('DOMContentLoaded', function () {
  fetch('/cpp-backend/src/ingredients.txt')
    .then(response => response.text())
    .then(data => {
      const lines = data.split('\n');

      // Loop through each line
      lines.forEach(line => {
        // Split the line into individual values
        const [name, quantity, importance, day, month, year] = line.split(' ');

        // Create description
        const description = quantity + ' ' + importance;
        console.log(document.getElementById('ingredients'));
        document.getElementById('ingredients').innerHTML += ('<dt>' + name + '</dt> <dd>' + description + '</dd>');
      })
    })
});

//add a new ingredient
document.getElementById('ingredient_submit').addEventListener('click', function () {
  // initializing a JavaScript object
  /*const new_ingredient = {
    name: document.getElementById("ingredient_name").value,
    quantity: document.getElementById("ingredient_quantity").value,
    unit: document.getElementById("ingredient_unit").value,
    date: document.getElementById("ingredient_expiration").value
  };
  var ingredientJSON = JSON.stringify(new_ingredient);
  */

  var date = new Date(document.getElementById("ingredient_date").value);
  var day = date.getDay();
  var month = date.getMonth();
  var year = date.getFullYear();

  //add the ingredient to the list
  //ingredient(document.getElementById("ingredient_name").value, document.getElementById("ingredient_quantity").value, document.getElementById("ingredient_date").value);

  // writing the JSON string content to a file
  //fs.writeFile("ingredients.json", ingredientJson, (error) => {
  // throwing the error
  // in case of a writing problem
  //if (error) {
  // logging the error
  //console.error(error);

  //throw error;
  // }

  //console.log("ingredients.json written correctly");
  //});
});

// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// No Node.js APIs are available in this process because
// `nodeIntegration` is turned off. Use `preload.js` to
// selectively enable features needed in the rendering
// process.

// Wait for the DOM to finish loading
document.addEventListener('DOMContentLoaded', function () {

  // Access the submit button and add event listener
  document.getElementById('ingredient_submit').addEventListener('click', function () {

    const ingredient = {
      name: "Banana",
      quantity: "99",
      importance: "10",
      day: 26,
      month: 4,
      year: 2024
    };

    console.log("Adding ingredient: " + ingredient)

    // send ingredient data to main.js 
    ipcRenderer.send('add-ingredient', ingredient);

    // receive message from main.js
    ipcRenderer.on('add-ingredient-result', (event, arg) => {
      console.log(arg);
    });
  });

});