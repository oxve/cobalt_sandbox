// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Flags: --allow-natives-syntax --liftoff --wasm-async-compilation

load('test/mjsunit/wasm/wasm-constants.js');
load('test/mjsunit/wasm/wasm-module-builder.js');

(function testLiftoffSync() {
  print(arguments.callee.name);
  const builder = new WasmModuleBuilder();
  builder.addFunction('i32_add', kSig_i_ii)
      .addBody([kExprGetLocal, 0, kExprGetLocal, 1, kExprI32Add])
      .exportFunc();

  const instance = builder.instantiate();

  assertTrue(%IsLiftoffFunction(instance.exports.i32_add));
})();

async function testLiftoffAsync() {
  print(arguments.callee.name);
  const builder = new WasmModuleBuilder();
  builder.addFunction('i32_add', kSig_i_ii)
      .addBody([kExprGetLocal, 0, kExprGetLocal, 1, kExprI32Add])
      .exportFunc();

  print('Compiling...');
  const module = await WebAssembly.compile(builder.toBuffer());
  print('Instantiating...');
  const instance = new WebAssembly.Instance(module);
  assertTrue(%IsLiftoffFunction(instance.exports.i32_add));
}

assertPromiseResult(testLiftoffAsync());
