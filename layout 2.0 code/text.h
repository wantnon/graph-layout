
/////////////////在屏幕上显示文本
//font可用值：
//GLUT_BITMAP_8_BY_13:一种固定宽度字体,每个字符都放在一个8x13像素的矩形框内
//GLUT_BITMAP_9_BY_15:一种固定宽度字体,每个字符都放在一个9x15像素的矩形框内
//GLUT_BITMAP_TIMES_ROMAN_10:一种10点均匀间距的Times Roman字体
//GLUT_BITMAP_TIMES_ROMAN_24:一种24点均匀间距的Times Roman字体
//GLUT_BITMAP_HELVETICA_10:一种10点均匀间距的Helvetica字体
//GLUT_BITMAP_HELVETICA_12:一种12点均匀间距的Helvetica字体
//GLUT_BITMAP_HELVETICA_18:一种18点均匀间距的Helvetica字体
void Begin2D(int style){//初始化2D环境
	//初始化2D环境
	glColor4f(1,1,1,1);//0 表示完全透明
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();//保存当前投影矩阵
	glLoadIdentity();
	//设置正投影
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	gluOrtho2D(0,viewport[2],0,viewport[3]);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//保存当前模型视图矩阵
	glLoadIdentity();
	//关闭光照
	glDisable(GL_LIGHTING);
	if(style==SCREEN_COOR){
		// 反转Y轴（朝下为正方向）(与窗口坐标一致)
		glScalef(1, -1, 1);
		// 将原点移动到屏幕左上方(与窗口坐标一致)
		glTranslatef(0, - viewport[3], 0);
	}else if(style==MATH_COOR){
	
	}

}
void End2D(){
	glEnable(GL_LIGHTING);
	// 恢复之前保存的模型视图矩阵
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	// 恢复之前保存的投影矩阵
	glPopMatrix();
	// 返回模型视图矩阵状态
	glMatrixMode(GL_MODELVIEW);
}
void Text(const char* text, float x, float y){  
	// 设置文字位置
	glRasterPos2f( x, y );
	// 依次描绘所有字符(使用显示列表)
	for(const char* c = text; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}