const { ipcRenderer } = require('electron');
// After make profile ->
document.addEventListener('DOMContentLoaded', function () {
document.getElementById('profile_submit').addEventListener('click', function() {
    event.preventDefault();
    let input1 = document.getElementById("username").value;

    // Getting checkbox values
    let checkboxes = document.querySelectorAll('input[name="restrictions"]:checked');
    let restrictionsArray = 0; // Initialize the integer

    checkboxes.forEach(function(checkbox) {
        // Calculate the bit position based on the value of the checkbox
        let bitPosition = 0;
        switch (checkbox.value) {
            case "Vegetarian":
                bitPosition = 11;
                break;
            case "Vegan":
                bitPosition = 10;
                break;
            case "Alcohol-Free":
                bitPosition = 9;
                break;
            case "Gluten":
                bitPosition = 8;
                break;
            case "Dairy":
                bitPosition = 7;
                break;
            case "Eggs":
                bitPosition = 6;
                break;
            case "Soy":
                bitPosition = 5;
                break;
            case "Wheat":
                bitPosition = 4;
                break;
            case "Fish":
                bitPosition = 3;
                break;
            case "Shellfish":
                bitPosition = 2;
                break;
            case "Tree Nuts":
                bitPosition = 1;
                break;
            case "Peanuts":
                bitPosition = 0;
                break;
            // Add more cases for additional checkboxes if needed
        }

        // Set the corresponding bit to 1 using bitwise OR
        restrictionsArray |= 1 << bitPosition;
    });
    console.log(input1);
    console.log(restrictionsArray);

    // send username, birthday, and restrictions to main.js 
    ipcRenderer.send('asynchronous-message', { 'input1': input1, 'input2': restrictionsArray});

    // receive message from main.js
    ipcRenderer.on('asynchronous-reply', (event, arg) => {
        // Address of native addon
        const { masterProfile } = require('./cpp-backend/build/Release/addon.node');

        // Calling functions of native addon
        var result = masterProfile(arg['input1'], restrictionsArray);

        document.getElementById('tag_result').innerHTML =
            "C++ Native addon add() result (IPC): " + result;
    });
});
})