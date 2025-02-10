// We register ops with a higher priority dispatch key (BackendSelect) than the usual backend-specific keys (e.g. CPU)
// which makes calls to the factory functions dispatch to here.
// We then 'manually' compute a lower-priority to re-dispatch to (e.g. CPU) to get to the eventually correct backend.
// ${generated_comment}

#define TORCH_ASSERT_ONLY_METHOD_OPERATORS
#include <ATen/core/Tensor.h>
#include <ATen/core/dispatch/DispatchKeyExtractor.h>
#include <torch/library.h>
#include <ATen/ops/is_managed_ops.h>
#include <ATen/ops/_manage_memory_ops.h>

#ifndef AT_PER_OPERATOR_HEADERS
#include <ATen/Operators.h>
#else

${ops_headers}
#endif

namespace at {

namespace {

bool is_managed(const Tensor& self, c10::optional<at::Device> device) {
  // TODO: fetch scalar type from Tensor? But it doesn't really matter...
  DispatchKeySet _dk = c10::DispatchKeySet(c10::computeDispatchKey(c10::nullopt, self.layout(), device.value_or(at::kCUDA)));
  return at::_ops::is_managed::redispatch(_dk, self, device);
}

at::Tensor _manage_memory(const Tensor& self, c10::optional<at::Device> device) {
  DispatchKeySet _dk = c10::DispatchKeySet(c10::computeDispatchKey(c10::nullopt, self.layout(), device.value_or(at::kCUDA)));
  return at::_ops::_manage_memory::redispatch(_dk, self, device);
}

${backend_select_method_definitions}

TORCH_LIBRARY_IMPL(aten, BackendSelect, m) {
  ${backend_select_function_registrations};
  m.impl(TORCH_SELECTIVE_NAME("aten::is_managed"), TORCH_FN(is_managed));
  m.impl(TORCH_SELECTIVE_NAME("aten::_manage_memory"), TORCH_FN(_manage_memory));
}

} // namespace
} // at
