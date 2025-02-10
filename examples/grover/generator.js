const fs = require('fs');
const path = require('path');
const {grover} = require("./generator/grover.js");

let items = process.argv[2];
let sols = items.split(" ");
let nqubits = sols[0].length;
let nIters = Math.floor(Math.PI * Math.sqrt(Math.pow(2, nqubits)) / 4);

for(let i = 0; i < sols.length; i++) {
    targetState = sols[i];
    const content = grover(nqubits, targetState, nIters);
    const folderPath = path.join(__dirname, 'grover-' + nqubits);
    const filePath = path.join(folderPath, 'grover-' + nqubits + '-' + (i + 1) + '.qw');
    fs.mkdir(folderPath, { recursive: true }, (err) => {
        if (err) {
            console.error('Error creating folder:', err);
            return;
        }
        fs.writeFile(filePath, content, err => {
            if (err) {
                console.error(err);
            } else {
                // console.log('grover-' + nqubits + '-' + (i + 1) + '.qw' + ' is generated successfully');
            }
        });
    });
}