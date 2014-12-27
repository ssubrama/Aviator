// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 

#include "build/build_config.h"
#include "chrome/browser/importer/importer_uma.h"

namespace importer {

namespace {

// The enum used to register importer use.
enum ImporterTypeMetrics {
  IMPORTER_METRICS_UNKNOWN         = 0,
#if defined(OS_WIN)
  IMPORTER_METRICS_IE              = 1,
#endif
  IMPORTER_METRICS_FIREFOX2        = 2,  // obsolete
  IMPORTER_METRICS_FIREFOX3        = 3,
#if defined(OS_MACOSX)
  IMPORTER_METRICS_SAFARI          = 4,
#endif
  IMPORTER_METRICS_GOOGLE_TOOLBAR5 = 5,  // obsolete
  IMPORTER_METRICS_BOOKMARKS_FILE  = 6,
 
  IMPORTER_METRICS_GOOGLE_CHROME = 7, //added for champion import bookmark for chrome 

  // Insert new values here. Never remove any existing values, as this enum is
  // used to bucket a UMA histogram, and removing values breaks that.
  IMPORTER_METRICS_SIZE
};

}  // namespace

void LogImporterUseToMetrics(const std::string& metric_postfix,
                             ImporterType type) {
  ImporterTypeMetrics metrics_type = IMPORTER_METRICS_UNKNOWN;
  switch (type) {
    case TYPE_UNKNOWN:
      metrics_type = IMPORTER_METRICS_UNKNOWN;
      break;
#if defined(OS_WIN)
    case TYPE_IE:
      metrics_type = IMPORTER_METRICS_IE;
      break;
#endif
    case TYPE_FIREFOX:
      metrics_type = IMPORTER_METRICS_FIREFOX3;
      break;
#if defined(OS_MACOSX)
    case TYPE_SAFARI:
      metrics_type = IMPORTER_METRICS_SAFARI;
      break;
#endif
    case TYPE_BOOKMARKS_FILE:
      metrics_type = IMPORTER_METRICS_BOOKMARKS_FILE;
      break;
    
    /*Added for champion google chrome*/  
    case TYPE_CHROME:
      metrics_type = 	 IMPORTER_METRICS_GOOGLE_CHROME;
      break;
  }

  // Note: This leaks memory, which is the expected behavior as the factory
  // creates and owns the histogram.
  base::HistogramBase* histogram =
      base::LinearHistogram::FactoryGet(
          "Import.ImporterType." + metric_postfix,
          1,
          IMPORTER_METRICS_SIZE,
          IMPORTER_METRICS_SIZE + 1,
          base::HistogramBase::kUmaTargetedHistogramFlag);
  histogram->Add(metrics_type);
}

}  // namespace importer
