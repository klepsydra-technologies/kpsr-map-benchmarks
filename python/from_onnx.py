import numpy as np
import tvm
from tvm.relax.frontend.onnx import from_onnx
import onnx

onnx_model_name = "alexnet.onnx"
onnx_model = onnx.load(onnx_model_name)
mod = from_onnx(onnx_model, keep_params_in_input=False)

target = tvm.target.Target("llvm")
executable = tvm.relax.build(mod, target, exec_mode="compiled")
executable.export_library(onnx_model_name[:-4]+"so")
