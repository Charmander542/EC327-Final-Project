//const {ipcRenderer} = require('electron');
const fs = require("fs");

//expire();

//get current ingredient information
import data from './ingredients.json' assert {type: 'json'};
for(var i = 0; i < data.ingredients.length; i++){
  var item = data.ingredients[i];
  var description = item.quantity + ' ' + item.unit;
  console.log(document.getElementById('ingredients'));
  document.getElementById('ingredients').innerHTML += ('<dt>'+item.name+'</dt> <dd>'+description+'</dd>');
}

//add a new ingredient
document.getElementById('ingredient_submit').addEventListener('click', function() {
  // initializing a JavaScript object
  const new_ingredient = {
    name: document.getElementById("ingredient_name").value,
    quantity: document.getElementById("ingredient_quantity").value,
    unit: document.getElementById("ingredient_unit").value,
    date: document.getElementById("ingredient_expiration").value
  };
  var ingredientJSON = JSON.stringify(new_ingredient);
  // writing the JSON string content to a file
  fs.writeFile("ingredients.json", ingredientJson, (error) => {
    // throwing the error
    // in case of a writing problem
    if (error) {
      // logging the error
      console.error(error);

      throw error;
    }

    console.log("ingredients.json written correctly");
  });
});