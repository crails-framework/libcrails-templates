#ifndef  TEMPLATE_HPP
# define TEMPLATE_HPP

# include <crails/shared_vars.hpp>
# include "template_stream.hpp"

namespace Crails
{
  class Renderer;
  class RenderTarget;

  class Template
  {
  public:
    Template(const Renderer& renderer, RenderTarget& target, SharedVars& vars) : vars(vars), target(target), renderer(renderer)
    {}

    std::string partial(const std::string_view view, SharedVars vars = {});
    bool has_partial(const std::string_view view) const;
  protected:
    virtual std::string apply_post_render_filters(std::string&&);

    SharedVars&     vars;
    RenderTarget&   target;
    TemplateStream  stream;
  private:
    const Renderer& renderer;
  };
}

#endif
