#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Graphics {
namespace Constants {

//文件操作
const char PROJECT_ID[] = "Constants.NewProject";
const char FILE_ID[] = "Constants.File";
const char SAVE_ID[] = "Constants.Save";
const char SAVE_AS_ID[] = "Constants.SaveAs";
const char OPEN_ID[] = "Constants.Open";
const char CLEAR_ID[] = "Constants.Clear";
const char EXIT_ID[] = "Constants.Exit";

const char UNDO_ID[] = "Constants.Undo";
const char REDO_ID[] = "Constants.Redo";
const char CUT_ID[] = "Constants.Cut";
const char COPY_ID[] = "Constants.Copy";
const char PASTE_ID[] = "Constants.Paste";
const char CLEAR_PASTE_ID[] = "Constants.ClearPaste";

//控件编辑
const char EDIT_TEXT_ID[] = "Constants.EditText";
const char PROPERTY_EDIT_ID[] = "Constants.PropertyEdit";

const char ROTATE_LEFT_ID[] = "Constants.RotateLeft";
const char ROTATE_RIGHT_ID[] = "Constants.RotateRight";
const char BRING_FRONT_ID[] = "Constants.BringFront";
const char BRING_BACK_ID[] = "Constants.BringBack";
const char DELETE_ID[] = "Constants.Delte";
const char LOCK_ID[] = "Constants.Lock";
const char UNLOCK_ID[] = "Constants.UnLock";
const char DRAG_ABLE_ID[] = "Constants.Dragable";
const char LEFT_LINE_ID[] = "Constants.LeftLine";
const char RIGHT_LINE_ID[] = "Constants.RightLine";

//控件
const char ARROW_ID[] = "Constants.Arrow";
const char SQUARE_ID[] = "Constants.Square";
const char RECT_ID[] = "Constants.Rect";
const char ROUNDRECT_ID[] = "Constants.RoundRect";
const char CIRCLE_ID[] = "Constants.Circle";
const char ELLIPSE_ID[] = "Constants.Ellipse";
const char POLYGON_ID[] = "Constants.Polygon";
const char PARALLEOGRAM_ID[] = "Constants.Parallelogram";
const char LINE_ID[] =  "Constants.Line";
const char VECTOR_LINE_ID[] =  "Constants.VectorLine";
const char TEXT_ID[] =  "Constants.Text";
const char NODE_PORT_ID[] = "Constants.NodePort";
const char NODE_TRIANGLE_IN_ID[] = "Constants.NodeTriangleIn";
const char NODE_TRIANGLE_OUT_ID[] = "Constants.NodeTriangleOut";
const char NODE_HALF_CIRCLE_ID[] = "Constants.NodeHalfCricle";
const char NODE_CIRCLE_ID[] = "Constants.NodeCircle";
const char ANNOTATION_ID[] = "Constants.Annotation";
const char PARALLEL_ID[] = "Constants.Parallel";
const char LOOP_DOWN_ID[] = "Constants.LoopDown";
const char LOOP_UP_ID[] = "Constants.LoopUp";

//状态图
const char STATE_START_ID[] = "Constants.State.Start";
const char STATE_END_ID[] = "Constants.State.End";
const char STATE_PROCESS_ID[] = "Constants.State.Process";

//遮罩
const char MASK_RECT_ID[] = "Constants.Mask.Rect";
const char MASK_BOUND_RECT_ID[] = "Constants.Mask.BoundRect";
const char MASK_CIRCLE_ID[] = "Constants.Mask.Circle";

//模式
const char BUILD_MODEL_ID[] = "Constants.BuildModel";
const char SIMLUATE_ID[] = "Constants.Simluate";

//端口
const char NODE_PROCESS_ID[] = "Constants.NodeProcess";

//【控件】
const char FULL_SCREEN_ID[] = "Constants.FullScreen";
const char HIDE_ICON_ID[] = "Constants.HideIcon";
const char HIDE_TOOL_ID[] = "Constants.HideToolBox";
const char HIDE_SIMULATE_ID[] = "Constants.HideSimulate";
const char HIDE_WORKSPACE_ID[] = "Constants.HideWorkspace";

//【帮助】
const char ASSISANT_KEY_LIST[] = "Constants.AssisantKeyList";
const char TEC_SUPPORT_ID[] = "Constants.TecSupport";
const char ABOUT_ID[] = "Constants.About";

//【数据库】
const char DATABASE_VIEW[] = "Constants.DatabaseView";
const char DATABASE_REFRESH[] = "Constants.DatabaseRefresh";

//【工作区间】
const char CLOSE_WORKSPACE[] = "Workspace.Close";
const char SAVE_WORKSPACE[] = "Workspace.Save";
const char CLOSE_LEFT_WORKSPACE[] = "Workspace.CloseLeft";
const char CLOSE_RIGHT_WORKSPACE[] = "Workspace.CloseRight";

//【节点】
const char NODE_EDIT_ID[] = "Constants.Node.Edit";
const char NODE_DELETE_ID[] = "Constants.Node.Delete";

//【Menu/右键菜单】
const char MENUBAR_FILE_MENU[] = "Constants.Menu.FileMenu";
const char MENUBAR_EDIT_MENU[] = "Constants.Menu.EditMenu";
const char MENUBAR_ITEM_MENU[] = "Constants.Menu.ItemMenu";
const char MENUBAR_WIDGET_MENU[] = "Constants.Menu.WidgetMenu";
const char MENUBAR_HELP_MENU[] = "Constants.Menu.HelpMenu";

const char MENU_MYPAGE_SWITCH[] = "Constants.Menu.PageSwitchMenu";
const char MENU_GRAPHICS_VIEW[] = "Constants.Menu.GraphicsViewMenu";
const char MENU_ITEM_RIGHT_MENU[] = "Constants.Menu.ItemRightMenu";

const char MENU_SERVICE_INPUT[] = "Constants.Menu.ServiceInputMenu";
const char MENU_NODE_PORT[] = "Constants.Menu.NodePort";

} // namespace Constants
} // namespace Graphics
#endif // CONSTANTS_H
