#include <crails/template.hpp>
#include <crails/renderer.hpp>

#undef NDEBUG
#include <cassert>

using namespace Crails;

struct TemplateA : public Template
{
  TemplateA(const Renderer& r, RenderTarget& t, SharedVars& v) : Template(r, t, v) {}
  std::string_view render() const { return "Simple template render"; }
};

struct TemplateB : public Template
{
  TemplateB(const Renderer& r, RenderTarget& t, SharedVars& v) : Template(r, t, v) {}

  std::string render()
  {
    std::string result = "Template render with partial: ";
    assert(has_partial("simple-view") == true);
    result += partial("simple-view", vars);
    return result + '!';
  }
};

struct TestRenderer : public Renderer
{
  TestRenderer()
  {
    templates.emplace("simple-view", [](const Renderer& r, RenderTarget& t, SharedVars& v)
    {
      TemplateA tpl(r, t, v);
      t.set_body(tpl.render());
    });
    templates.emplace("view-with-partial", [](const Renderer& r, RenderTarget& t, SharedVars& v)
    {
      TemplateB tpl(r, t, v);
      t.set_body(tpl.render());
    });
  }

  std::string_view get_name() const override { return "TestRenderer"; }

  const std::vector<std::string>& get_mimetypes() const override
  {
    static const std::vector<std::string> result = {};
    return result;
  }

  void render_template(const std::string& view, RenderTarget& target, SharedVars& vars) const override
  {
    auto tpl = templates.find(view);
    (*tpl).second(*this, target, vars);
  }
};

int main()
{
  TestRenderer renderer;
  SharedVars vars;

  {
    RenderString output;

    renderer.render_template("simple-view", output, vars);
    assert(output.value() == "Simple template render");
  }

  {
    RenderString output;

    renderer.render_template("view-with-partial", output, vars);
    assert(output.value() == "Template render with partial: Simple template render!");
  }

  return 0;
}
