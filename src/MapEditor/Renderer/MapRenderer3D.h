
#ifndef __MAP_RENDERER_3D_H__
#define __MAP_RENDERER_3D_H__

#include "MapEditor/SLADEMap/SLADEMap.h"
#include "General/ListenerAnnouncer.h"
#include "MapEditor/Edit/Edit3D.h"

class ItemSelection;
class GLTexture;
class Polygon2D;

namespace Game { class ThingType; }

class MapRenderer3D : public Listener
{
public:
	// Structs
	enum
	{
	    // Common flags
	    TRANSADD	= 2,

	    // Quad/flat flags
	    SKY		= 4,
		DRAWBOTH	= 128,

	    // Quad flags
	    BACK	= 8,
	    UPPER	= 16,
	    LOWER	= 32,
	    MIDTEX	= 64,

	    // Flat flags
	    CEIL	 = 8,
		FLATFLIP = 16,

	    // Thing flags
	    ICON	= 4,
	    DRAWN	= 8,
		ZETH	= 16,
	};
	struct gl_vertex_t
	{
		float x, y, z;
		float tx, ty;
	};
	struct quad_3d_t
	{
		gl_vertex_t	points[4];
		rgba_t		colour;
		rgba_t		fogcolour;
		uint8_t		light;
		GLTexture*	texture;
		uint8_t		flags;
		float		alpha;
		int         control_line;
		int         control_side;

		quad_3d_t()
		{
			colour.set(255, 255, 255, 255, 0);
			texture = nullptr;
			flags = 0;
			control_line = -1;
			control_side = -1;
		}
	};
	struct line_3d_t
	{
		vector<quad_3d_t>	quads;
		long				updated_time;
		bool				visible;
		MapLine*			line;

		line_3d_t() { updated_time = 0; visible = true; }
	};
	struct thing_3d_t
	{
		uint8_t					flags;
		Game::ThingType const*	type;
		MapSector*				sector;
		float					z;
		float					height;
		GLTexture*				sprite;
		long					updated_time;

		thing_3d_t()
		{
			flags = 0;
			type = nullptr;
			sector = nullptr;
			sprite = nullptr;
			z = 0;
			updated_time = 0;
		}
	};
	struct flat_3d_t
	{
		uint8_t		flags;
		uint8_t		light;
		rgba_t		colour;
		rgba_t		fogcolour;
		GLTexture*	texture;
		plane_t		plane;
		float		base_alpha;
		float		alpha;
		MapSector*	sector;
		MapSector*	control_sector;
		int			extra_floor_index;
		long		updated_time;
		unsigned	vbo_offset;

		flat_3d_t()
		{
			light = 255;
			texture = nullptr;
			updated_time = 0;
			flags = 0;
			base_alpha = 1.0f;
			alpha = 1.0f;
			sector = nullptr;
		}
	};

	MapRenderer3D(SLADEMap* map = nullptr);
	~MapRenderer3D();

	bool	fullbrightEnabled() { return fullbright; }
	bool	fogEnabled() { return fog; }
	void	enableFullbright(bool enable = true) { fullbright = enable; }
	void	enableFog(bool enable = true) { fog = enable; }
	int		itemDistance() { return item_dist; }
	void	enableHilight(bool render) { render_hilight = render; }
	void	enableSelection(bool render) { render_selection = render; }

	bool	init();
	void	refresh();
	void	clearData();
	void	buildSkyCircle();

	quad_3d_t*	getQuad(MapEditor::Item item);
	flat_3d_t*	getFlat(MapEditor::Item item);

	// Camera
	void	cameraMove(double distance, bool z = true);
	void	cameraTurn(double angle);
	void	cameraMoveUp(double distance);
	void	cameraStrafe(double distance);
	void	cameraPitch(double amount);
	void	cameraUpdateVectors();
	void	cameraSet(fpoint3_t position, fpoint2_t direction);
	void	cameraSetPosition(fpoint3_t position);
	void	cameraApplyGravity(double mult);
	void	cameraLook(double xrel, double yrel);

	double		camPitch() { return cam_pitch; }
	fpoint3_t	camPosition() { return cam_position; }
	fpoint2_t	camDirection() { return cam_direction; }

	// -- Rendering --
	void	setupView(int width, int height);
	void	setLight(rgba_t& colour, uint8_t light, float alpha = 1.0f);
	void	setFog(rgba_t &fogcol, uint8_t light);
	void	renderMap();
	void	renderSkySlice(float top, float bottom, float atop, float abottom, float size, float tx = 0.125f, float ty = 2.0f);
	void	renderSky();

	// Flats
	void	updateFlatTexCoords(unsigned index, unsigned flat_index);
	void	updateSector(unsigned index);
	void	updateSectorFlats(unsigned index);
	void	updateSectorVBOs(unsigned index);
	bool	isSectorStale(unsigned index);
	void	renderFlat(flat_3d_t* flat);
	void	renderFlats();
	void	renderFlatSelection(const ItemSelection& selection, float alpha = 1.0f);

	// Walls
	void	setupQuad(quad_3d_t* quad, fseg2_t seg, double top, double bottom);
	void	setupQuad(quad_3d_t* quad, fseg2_t seg, plane_t top, plane_t bottom);
	void	setupQuadTexCoords(quad_3d_t* quad, int length, double o_left, double o_top, double h_top, double h_bottom, bool pegbottom = false, double sx = 1, double sy = 1);
	void	updateLine(unsigned index);
	void	renderQuad(quad_3d_t* quad, float alpha = 1.0f);
	void	renderWalls();
	void	renderTransparentWalls();
	void	renderWallSelection(const ItemSelection& selection, float alpha = 1.0f);

	// Things
	void	updateThing(unsigned index, MapThing* thing);
	void	renderThings();
	void	renderThingSelection(const ItemSelection& selection, float alpha = 1.0f);

	// VBO stuff
	void	updateFlatsVBO();
	void	updateWallsVBO();

	// Visibility checking
	void	quickVisDiscard();
	float	calcDistFade(double distance, double max = -1);
	void	checkVisibleQuads();
	void	checkVisibleFlats();

	// Hilight
	MapEditor::Item	determineHilight();
	void				renderHilight(MapEditor::Item hilight, float alpha = 1.0f);

	// Listener stuff
	void	onAnnouncement(Announcer* announcer, string event_name, MemChunk& event_data);

private:
	SLADEMap*	map;
	bool		fullbright;
	bool		fog;
	GLTexture*	tex_last;
	unsigned	n_quads;
	unsigned	n_flats;
	int			flat_last;
	bool		render_hilight;
	bool		render_selection;
	rgba_t		fog_colour_last;
	float		fog_depth_last;

	// Visibility
	vector<float>	dist_sectors;

	// Camera
	fpoint3_t	cam_position;
	fpoint2_t	cam_direction;
	double		cam_pitch;
	double		cam_angle;
	fpoint3_t	cam_dir3d;
	fpoint3_t	cam_strafe;
	double		gravity;
	int			item_dist;

	// Map Structures
	vector<line_3d_t>	lines;
	quad_3d_t**			quads;
	vector<quad_3d_t*>	quads_transparent;
	vector<thing_3d_t>	things;
	vector<vector<flat_3d_t> >	sector_flats;
	vector<flat_3d_t*>	flats;

	// VBOs
	unsigned	vbo_flats;
	unsigned	vbo_walls;

	// Sky
	struct gl_vertex_ex_t
	{
		float x, y, z;
		float tx, ty;
		float alpha;
	};
	string		skytex1;
	string		skytex2;
	rgba_t		skycol_top;
	rgba_t		skycol_bottom;
	fpoint2_t	sky_circle[32];
};

#endif//__MAP_RENDERER_3D_H__
