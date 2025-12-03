import numpy as np
import tvm
from tvm.relax.frontend.onnx import from_onnx
import onnx
import os

onnx_model_name = "mobilenetv2-7.onnx"
onnx_model = onnx.load(onnx_model_name)
mod = from_onnx(onnx_model, keep_params_in_input=False)

#target = tvm.target.Target(f"llvm -num-cores {os.cpu_count()}")
isTuned = True
filename = onnx_model_name[:-4]+"so"
if isTuned:
    #TOTAL_TRIALS = 8000
    #mod = tvm.relax.get_pipeline("default", target=target, total_trials=TOTAL_TRIALS)(mod)
    mod = tvm.relax.pipeline.static_shape_tuning_pipeline(
            total_trials=1000,
            target=f"llvm -num-cores {os.cpu_count()}",
            work_dir="tuning_logs",
            cpu_weight_prepack=True,
        )(mod)
    filename = onnx_model_name[:-5]+"_optimized3.so"
    print("Model done")

executable = tvm.relax.build(mod, target, exec_mode="compiled")

print(executable)
executable.export_library(filename)
