import fs from 'fs/promises';

const filePath = 'networkcoding.qw';
const progName = "NETWORK_CODING";
const nqubits = 14;

async function clearFile() {
    try {
        await fs.writeFile(filePath, "");
    } catch (err) {
        console.error('Failed to clear:', err);
    }
}
async function appendContent(content) {
    try {
        await fs.appendFile(filePath, content);
    } catch (err) {
        console.error('Failed to append:', err);
    }
}
async function header() {
    let prog = "prog " + progName + " is\n";
    let qubits = "";
    for (let i = 0; i < nqubits; i++) {
        qubits += "q" + i + ((i == (nqubits - 1)) ? "" : ", ");
    }
    prog += "var\n\t" + qubits + " : qubit;\n";
    await appendContent(prog);
}

async function init(){
    let prog = "init\n";
    for (let i = 0; i < nqubits; i++) {
        prog += "\tq" + i + " := |0>;\n";
    }
    await appendContent(prog);
}

async function prop() {
    let prog = "prop\n";
    prog += "\ttarget := P(q1, q4, |phi+>) and P(q0, q5, |phi+>);\n";
    await appendContent(prog);
}
async function con(n1, n2, n3) {
    let prog = "";
    prog += "\tq" + n1 + ", q" + n2 + " := CX[q" + n1 + ", q" + n2 + "];\n";
    prog += "\tif M[q" + n2 + "] = 1 then q" + n3 + " := X[" + "q" + n3 + "]; else skip; fi;\n";
    await appendContent(prog);
}

async function add(n1, n2, n3, n4) {
    let prog = "";
    prog += "\tq" + n1 + ", q" + n3 + " := CX[q" + n1 + ", q" + n3 + "];\n";
    prog += "\tq" + n2 + ", q" + n3 + " := CX[q" + n2 + ", q" + n3 + "];\n";
    prog += "\tif M[q" + n3 + "] = 1 then q" + n4 + " := X[" + "q" + n4 + "]; else skip; fi;\n";
    await appendContent(prog);
}

async function fanout(n1, n2, n3, n4, n5) {
    await con(n1, n2, n3);
    await con(n1, n4, n5);
}

async function rem(n1, n2) {
    let prog = "";
    prog += "\tq" + n1 + " := H[q" + n1 + "];\n";
    prog += "\tif M[q" + n1 + "] = 1 then q" + n2 + " := Z[q" + n2 + "]; else skip; fi;\n";
    await appendContent(prog);
}

async function remadd(n3, n1, n2) {
    let prog = "";
    prog += "\tq" + n3 + " := H[q" + n3 + "];\n";
    prog += "\tif M[q" + n3 + "] = 1 then q" + n1 + " := Z[q" + n1 + "]; q" + n2 + " := Z[q" + n2 + "]; else skip; fi;\n";
    await appendContent(prog);
}

async function begin() {
    await appendContent("begin\n");
    await appendContent("\tq0 := H[q0];\n");
    await appendContent("\tq0, q1 := CX[q0, q1];\n");
    await appendContent("\tq2 := H[q2];\n");
    await appendContent("\tq2, q3 := CX[q2, q3];\n");
    await appendContent("\tq4 := H[q4];\n");
    await appendContent("\tq4, q5 := CX[q4, q5];\n");
    await appendContent("\tq6 := H[q6];\n");
    await appendContent("\tq6, q7 := CX[q6, q7];\n");
    await appendContent("\tq8 := H[q8];\n");
    await appendContent("\tq8, q9 := CX[q8, q9];\n");
    await appendContent("\tq10 := H[q10];\n");
    await appendContent("\tq10, q11 := CX[q10, q11];\n");
    await appendContent("\tq12 := H[q12];\n");
    await appendContent("\tq12, q13 := CX[q12, q13];\n");
}

async function end() {
    await appendContent("end\n\n");
    await appendContent("pcheck in " + progName + " with 'P=? [F \"target\"]' --backend=Storm .\n\n");
    await appendContent("search in " + progName + " with =>! such that P(q1, q4, |phi+>) and P(q0, q5, |phi+>) .\n\n");
    await appendContent("psearch in " + progName + " with =>! such that P(q1, q4, |phi+>) and P(q0, q5, |phi+>) .");
}
async function generateNetworkCoding() {
    await clearFile();
    await header();
    await init();
    await prop();
    await begin();
    await con(0,2, 3);
    await con(4, 6, 7);
    await add(3, 7, 8, 9) ;
    await fanout(9, 10, 11, 12, 13);
    await appendContent("\tq13, q5 := CX[q13, q5];\n");
    await appendContent("\tq11, q1 := CX[q11, q1];\n");
    await rem(11, 9);
    await rem(13, 9);
    await remadd(9, 3, 7);
    await rem(3, 0);
    await rem(7, 4);
    await end();
}

await generateNetworkCoding();