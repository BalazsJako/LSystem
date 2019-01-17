//======================
// Sample LS file
//======================

var
	Quad
	glVertex
	glBegin
	glEnd

axiom:
	Quad(vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(1.0, 1.0), vec2(0.0, 1.0));

subdivide:
	Quad(vec2 v0, vec2 v1, vec2 v2, vec2 v3) ? length(v2 - v0) > 0.2 ->
		Quad(v0, (v0 + v1) * 0.5, (v0 + v1 + v2 + v3) * 0.25, (v0 + v3) * 0.5)
		Quad((v0 + v1) * 0.5, v1, (v1 + v2) * 0.5, (v0 + v1 + v2 + v3) * 0.25)
		Quad((v0 + v1 + v2 + v3) * 0.25, (v1 + v2) * 0.5, v2, (v2 + v3) * 0.5)
		Quad((v0 + v3) * 0.5, (v0 + v1 + v2 + v3) * 0.25, (v2 + v3) * 0.5, v3)
		;

emit:
	Quad(vec2 v0, vec2 v1, vec2 v2, vec2 v3) ? length(v2 - v0) <= 0.2 ->
		glBegin()
		glVertex(v0.x, v0.y)
		glVertex(v1.x, v1.y)
		glVertex(v2.x, v2.y)
		glVertex(v3.x, v3.y)
		glEnd()
	;
