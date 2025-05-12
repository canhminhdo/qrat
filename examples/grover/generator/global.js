targetState5  = "11000";
targetState10 = "0101101001";
targetState15 = "110000001101010";
targetState20 = "00001010111011000000";
targetState = targetState20;
nqubits = targetState.length;
nIters = Math.floor(Math.PI / 4 * Math.sqrt(Math.pow(2, nqubits)));

module.exports = {nqubits, targetState, nIters};