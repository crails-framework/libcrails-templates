#include "template.hpp"
#include <crails/renderer.hpp>

using namespace Crails;
using namespace std;

std::string Template::partial(const std::string& view, SharedVars vars_)
{
  RenderString partial_target;
  auto templates = renderer.get_templates();
  auto partial_template = templates.find(view);

  if (partial_template == templates.end())
    throw MissingTemplate(view);
  else
  {
    SharedVars duplicate = vars;

    for (auto& var : vars_)
      duplicate[var.first] = var.second;
    partial_template->second(renderer, partial_target, duplicate);
  }
  return partial_target.c_str();
}
