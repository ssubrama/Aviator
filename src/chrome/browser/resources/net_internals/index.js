// Copyright (c) [2013-2015] WhiteHat. All Rights Reserved. WhiteHat contributions included 
// in this file are licensed under the BSD-3-clause license (the "License") included in 
// the WhiteHat-LICENSE file included in the root directory of the distributed source code 
// archive. Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF
// ANY KIND, either express or implied. See the License for the specific language governing 
// permissions and limitations under the License. 
// champion removed(chromeos_view removed)
<include src="util.js"/>
<include src="table_printer.js"/>
<include src="view.js"/>
<include src="mouse_over_help.js"/>
<include src="tab_switcher_view.js"/>
<include src="import_view.js"/>
<include src="capture_view.js"/>
<include src="export_view.js"/>
<include src="http_cache_view.js"/>
<include src="test_view.js"/>
<include src="hsts_view.js"/>
<include src="browser_bridge.js"/>
<include src="events_tracker.js"/>
<include src="source_tracker.js"/>
<include src="resizable_vertical_split_view.js"/>
<include src="main.js"/>
<include src="time_util.js"/>
<include src="log_util.js"/>
<include src="capture_status_view.js"/>
<include src="loaded_status_view.js"/>
<include src="halted_status_view.js"/>
<include src="top_bar_view.js"/>
<include src="dns_view.js"/>
<include src="source_filter_parser.js"/>
<include src="source_row.js"/>
<include src="events_view.js"/>
<include src="waterfall_view.js"/>
<include src="waterfall_row.js"/>
<include src="details_view.js"/>
<include src="source_entry.js"/>
<include src="horizontal_scrollbar_view.js"/>
<include src="top_mid_bottom_view.js"/>
<include src="timeline_data_series.js"/>
<include src="timeline_graph_view.js"/>
<include src="timeline_view.js"/>
<include src="log_view_painter.js"/>
<include src="log_grouper.js"/>
<include src="proxy_view.js"/>
<include src="quic_view.js"/>
<include src="socket_pool_wrapper.js"/>
<include src="sockets_view.js"/>
<include src="spdy_view.js"/>
<include src="modules_view.js"/>
<include src="logs_view.js"/>
<include src="prerender_view.js"/>
<include src="bandwidth_view.js"/>
<include src="cros_log_visualizer_view.js"/>
<include src="cros_log_entry.js"/>
<include src="cros_log_visualizer.js" />
<include src="cros_log_marker.js" />

document.addEventListener('DOMContentLoaded', function() {
  MainView.getInstance();  // from main.js
});
