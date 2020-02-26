import sys
import os
import shlex
import subprocess
read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'
if read_the_docs_build:
    subprocess.call('doxygen', shell=True)
extensions = ['breathe']
breathe_projects = { 'XK92-ISS': 'xml' }
breathe_default_project = "XK92-ISS"
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'
project = u'XK92-ISS'
copyright = u'2015, XK92-ISS'
author = u'XK92-ISS'
version = '1.0'
release = '1.0'
language = None
exclude_patterns = ['_build']
pygments_style = 'sphinx'
todo_include_todos = False
html_static_path = ['_static']
htmlhelp_basename = 'XK92-ISSdoc'
latex_elements = {
}
latex_documents = [
  (master_doc, 'XK92-ISS.tex', u'XK92-ISS Documentation',
   u'XK92-ISS', 'manual'),
]
