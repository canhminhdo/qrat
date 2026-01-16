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
    prog += "\tCX[q" + n1 + ", q" + n2 + "];\n";
    prog += "\tif M[q" + n2 + "] = 1 then X[" + "q" + n3 + "]; else skip; fi;\n";
    await appendContent(prog);
}

async function add(n1, n2, n3, n4) {
    let prog = "";
    prog += "\tCX[q" + n1 + ", q" + n3 + "];\n";
    prog += "\tCX[q" + n2 + ", q" + n3 + "];\n";
    prog += "\tif M[q" + n3 + "] = 1 then X[" + "q" + n4 + "]; else skip; fi;\n";
    await appendContent(prog);
}

async function fanout(n1, n2, n3, n4, n5) {
    await con(n1, n2, n3);
    await con(n1, n4, n5);
}

async function rem(n1, n2) {
    let prog = "";
    prog += "\tH[q" + n1 + "];\n";
    prog += "\tif M[q" + n1 + "] = 1 then Z[q" + n2 + "]; else skip; fi;\n";
    await appendContent(prog);
}

async function remadd(n3, n1, n2) {
    let prog = "";
    prog += "\tH[q" + n3 + "];\n";
    prog += "\tif M[q" + n3 + "] = 1 then Z[q" + n1 + "]; Z[q" + n2 + "]; else skip; fi;\n";
    await appendContent(prog);
}

async function begin() {
    await appendContent("begin\n");
    await appendContent("\tH[q0];\n");
    await appendContent("\tCX[q0, q1];\n");
    await appendContent("\tH[q2];\n");
    await appendContent("\tCX[q2, q3];\n");
    await appendContent("\tH[q4];\n");
    await appendContent("\tCX[q4, q5];\n");
    await appendContent("\tH[q6];\n");
    await appendContent("\tCX[q6, q7];\n");
    await appendContent("\tH[q8];\n");
    await appendContent("\tCX[q8, q9];\n");
    await appendContent("\tH[q10];\n");
    await appendContent("\tCX[q10, q11];\n");
    await appendContent("\tH[q12];\n");
    await appendContent("\tCX[q12, q13];\n");
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
    await appendContent("\tCX[q13, q5];\n");
    await appendContent("\tCX[q11, q1];\n");
    await rem(11, 9);
    await rem(13, 9);
    await remadd(9, 3, 7);
    await rem(3, 0);
    await rem(7, 4);
    await end();
}

await generateNetworkCoding();