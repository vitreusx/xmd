import jinja2
from dataclasses import dataclass
from typing import List, Tuple
from pathlib import Path
from glob import glob
import subprocess
import re
from tempfile import NamedTemporaryFile


@dataclass
class Signature:
    name: str
    fields: List[Tuple[str, str]]


class Renderer:
    MAX_FIELDS = 8
    MAX_TEMPLATE_PARAMS = 16
    MAX_NAMESPACES = 8
    
    len = len
    zip = zip

    @staticmethod
    def macro_sig(n: int) -> Signature:
        name = "name"
        fields = [(f"T{idx}", f"x{idx}") for idx in range(1, n + 1)]
        return Signature(name, fields)

    @staticmethod
    def minify(text: str) -> str:
        return re.sub(r"\n", "", text)

    @staticmethod
    def clang_format(text: str) -> str:
        with NamedTemporaryFile("r+", delete=False) as f:
            path = f.name
        with open(path, "w") as f:
            f.write(text)
        subprocess.run(["clang-format", "-i", path])
        with open(path, "r") as f:
            return f.read()

    @staticmethod
    def as_macro_body(text: str) -> str:
        text = Renderer.clang_format(text)
        return re.sub(r"\n", r"\\\n", text)

    def __init__(self) -> None:
        searchpath = [".", "_extras", "_templates"]
        self.loader = jinja2.FileSystemLoader(searchpath=searchpath)

        self.env = jinja2.Environment(loader=self.loader)
        self.env.filters["minify"] = self.minify
        self.env.filters["as_macro_body"] = self.as_macro_body

        self.def_params = Renderer.__dict__

    def render(self, template_path: Path, out_path: Path, **params):
        template = self.env.get_template(str(template_path))
        code = template.render({**self.def_params, **params})
        code = self.clang_format(code)

        out_path.parent.mkdir(parents=True, exist_ok=True)
        with open(out_path, "w") as f:
            f.write(code)

    def render_all(self):
        templates = [*glob("_templates/**.j2"), *glob("_templates/**/*.j2")]

        for template_path in templates:
            template_path = Path(template_path)
            out_path = template_path.with_suffix("")
            out_path = Path("include/gentypes") / out_path.relative_to("_templates")
            self.render(template_path, out_path)


def main():
    renderer = Renderer()
    renderer.render_all()


if __name__ == "__main__":
    main()
