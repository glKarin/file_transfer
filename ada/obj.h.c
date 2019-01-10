#ifndef _KARIN_OBJ_H
#define _KARIN_OBJ_H

#include "../gl/mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENDLF '\n'

	int ada_parse(mesh_s *r, const char *file)
	{
		FILE *f;
		int fp;
		int ch;
		char *p;
		int len;
		int vc, vtc, vnc, gc, fc;
		int i, j;
		float *vs, *vts, *vns;
		int fs[9];
		material_s *m;
		point_s *pa;

		if(!r || !file)
			return -1;
		
        f = fopen("E:\\pro\\qt\\file_transfer\\ada\\AdaORCObj.obj", "r");

		// get vertex|texcoord|normal|triangle|mesh count
		vc = vtc = vnc = gc = fc = 0;
		fp = ftell(f);
		while(1)
		{
			ch = fgetc(f);
			if(ch == ENDLF || ch == EOF)
			{
				len = ftell(f) - fp;
				if(len == 0)
					break;
				p = (char *)calloc(len, 1);
				fseek(f, fp, SEEK_SET);
				fread(p, len, 1, f);
				if(p[len - 2] == '\r')
					p[len - 2] = '\0';
				p[len - 1] = '\0';
				i = 0;
				while(1)
				{
					if(p[i] == ' ' || p[i] == '\0')
						break;
					i++;
				}
				if(i != 0)
				{
					if(strncmp("v", p, i) == 0)
						vc++;
					else if(strncmp("vt", p, i) == 0)
						vtc++;
					else if(strncmp("vn", p, i) == 0)
						vnc++;
					else if(strncmp("f", p, i) == 0)
						fc++;
					else if(strncmp("g", p, i) == 0)
						gc++;
				}
				free(p);
				fp = ftell(f);
			}
			if(ch == EOF)
				break;
		}

        //printf("%d %d %d %d %d\n", vc, vtc, vnc, gc, fc);
		newmesh(r, gc);
		vs = (float *)calloc(vc * 3, sizeof(float));
		vts = (float *)calloc(vtc * 2, sizeof(float));
		vns = (float *)calloc(vnc * 3, sizeof(float));

		// get all vertex|texcoord|normal data
		fseek(f, 0, SEEK_SET);
		vc = vtc = vnc = gc = fc = 0;
		fp = ftell(f);
		while(1)
		{
			ch = fgetc(f);
			if(ch == ENDLF || ch == EOF)
			{
				len = ftell(f) - fp;
				if(len == 0)
					break;
				p = (char *)calloc(len, 1);
				fseek(f, fp, SEEK_SET);
				fread(p, len, 1, f);
				if(p[len - 2] == '\r')
					p[len - 2] = '\0';
				p[len - 1] = '\0';
				i = 0;
				while(1)
				{
					if(p[i] == ' ' || p[i] == '\0')
						break;
					i++;
				}
				if(i != 0)
				{
					if(strncmp("v", p, i) == 0)
					{
						sscanf(p + i, "%f %f %f", vs + vc * 3, vs + vc * 3 + 1, vs + vc * 3 + 2);
						vc++;
					}
					else if(strncmp("vt", p, i) == 0)
					{
						sscanf(p + i, "%f %f", vts + vtc * 2, vts + vtc * 2 + 1);
						vtc++;
					}
					else if(strncmp("vn", p, i) == 0)
					{
						sscanf(p + i, "%f %f %f", vns + vnc * 3, vns + vnc * 3 + 1, vns + vnc * 3 + 2);
						vnc++;
					}
					else if(strncmp("f", p, i) == 0)
					{
						fc++;
					}
					else if(strncmp("g", p, i) == 0)
					{
						gc++;
					}
				}
				free(p);
				fp = ftell(f);
			}
			if(ch == EOF)
				break;
		}

		// get material count of a mesh
		fseek(f, 0, SEEK_SET);
		vc = vtc = vnc = gc = fc = 0;
		fp = ftell(f);
		while(1)
		{
			ch = fgetc(f);
			if(ch == ENDLF || ch == EOF)
			{
				len = ftell(f) - fp;
				if(len == 0)
					break;
				p = (char *)calloc(len, 1);
				fseek(f, fp, SEEK_SET);
				fread(p, len, 1, f);
				if(p[len - 2] == '\r')
					p[len - 2] = '\0';
				p[len - 1] = '\0';
				i = 0;
				while(1)
				{
					if(p[i] == ' ' || p[i] == '\0')
						break;
					i++;
				}
				if(i != 0)
				{
					if(strncmp("f", p, i) == 0)
					{
						fc++;
					}
					else if(strncmp("g", p, i) == 0)
					{
						if(gc != 0)
						{
							newmat(r->materials + gc - 1, fc * 3);
							r->materials[gc - 1].use_color = false;
							fc = 0;
						}
						gc++;
					}
				}
				free(p);
				fp = ftell(f);
			}
			if(ch == EOF)
				break;
		}
		
		if(fc != 0)
		{
			newmat(r->materials + gc - 1, fc * 3);
		}

		// get material data of a mesh
		fseek(f, 0, SEEK_SET);
		vc = vtc = vnc = gc = fc = 0;
		fp = ftell(f);
		while(1)
		{
			ch = fgetc(f);
			if(ch == ENDLF || ch == EOF)
			{
				len = ftell(f) - fp;
				if(len == 0)
					break;
				p = (char *)calloc(len, 1);
				fseek(f, fp, SEEK_SET);
				fread(p, len, 1, f);
				if(p[len - 2] == '\r')
					p[len - 2] = '\0';
				p[len - 1] = '\0';
				i = 0;
				while(1)
				{
					if(p[i] == ' ' || p[i] == '\0')
						break;
					i++;
				}
				if(i != 0)
				{
					if(strncmp("f", p, i) == 0)
					{
						//printf("%d %d\n", gc, fc);
						sscanf(p + i, "%d/%d/%d %d/%d/%d %d/%d/%d",   fs, fs + 1, fs + 2,   fs + 3, fs + 4, fs + 5,   fs + 6, fs + 7, fs + 8);
						for(j = 0; j < 3; j++)
						{
							pa = m->points + fc * 3 + j;

							pa->vertex[0] = vs[fs[j * 3] * 3];
							pa->vertex[1] = vs[fs[j * 3] * 3 + 1];
							pa->vertex[2] = vs[fs[j * 3] * 3 + 2];
							pa->vertex[3] = 1;
							pa->texcoord[0] = vts[fs[j * 3 + 1] * 2];
							pa->texcoord[1] = vts[fs[j * 3 + 1] * 2 + 1];
							pa->normal[0] = vns[fs[j * 3 + 2] * 3];
							pa->normal[1] = vns[fs[j * 3 + 2] * 3 + 1];
							pa->normal[2] = vns[fs[j * 3 + 2] * 3 + 2];
							pa->color[0] = 
							pa->color[1] = 
							pa->color[2] = 0;
							pa->color[3] = 1;
						}
						fc++;
					}
					else if(strncmp("g", p, i) == 0)
					{
						fc = 0;
						m = r->materials + gc;
						gc++;
					}
				}
				free(p);
				fp = ftell(f);
			}
			if(ch == EOF)
				break;
		}

		free(vs);
		free(vts);
		free(vns);

		fclose(f);
		return 0;
	}

#ifdef __cplusplus
}
#endif

#endif
