#include "template.hpp"
#include <crails/renderer.hpp>

using namespace Crails;
using namespace std;
    
std::string Template::apply_post_render_filters(const std::string& value)
{
  return value;
}

std::string Template::partial(const std::string& view, SharedVars vars_)
{
  RenderString partial_target;
  auto templates = renderer.get_templates();
  auto partial_template = templates.find(view);

  if (partial_template == templates.end())
    throw MissingTemplate(view, &renderer);
  else
  {
    SharedVars duplicate = vars;

    for (auto& var : vars_)
      duplicate[var.first] = var.second;
    partial_template->second(renderer, partial_target, duplicate);
  }
  return partial_target.c_str();
}

bool Template::has_partial(const std::string& view) const
{
  auto templates = renderer.get_templates();

  return templates.find(view) != templates.end();
}
