// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#ifndef UI_VIEWS_EXAMPLES_EXAMPLE_BASE_H_
#define UI_VIEWS_EXAMPLES_EXAMPLE_BASE_H_

#include <string>

#include "base/basictypes.h"

namespace views {
class View;

namespace examples {

class ExampleBase {
 public:
  virtual ~ExampleBase();

  // Sub-classes should creates and add the views to the given parent.
  virtual void CreateExampleView(View* parent) = 0;

  const std::string& example_title() const { return example_title_; }
  View* example_view() { return container_; }

 protected:
  explicit ExampleBase(const char* title);

  View* container() { return container_; }

  // Prints a message in the status area, at the bottom of the window.
  void PrintStatus(const char* format, ...);

  // Converts an boolean value to "on" or "off".
  const char* BoolToOnOff(bool value) {
    return value ? "on" : "off";
  }

 private:
  // Name of the example - used as title in the combobox list.
  std::string example_title_;

  // The view that contains the views example.
  View* container_;

  DISALLOW_COPY_AND_ASSIGN(ExampleBase);
};

}  // namespace examples
}  // namespace views

#endif  // UI_VIEWS_EXAMPLES_EXAMPLE_BASE_H_
