#include "karin_nltviewer.h"
#include "SOIL/SOIL.h"
#include "img_src/nl_std.h"

#include <QDebug>

#define SCENE2D_RANGE 1

karin_NLTViewer::karin_NLTViewer(QWidget *parent) :
    karin_GLWidget(parent)
{
    memset(&m_mesh, 0, sizeof(mesh_s));
}

karin_NLTViewer::~karin_NLTViewer()
{
}

void karin_NLTViewer::init()
{
    karin_GLWidget::init();

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
}

void karin_NLTViewer::deinit()
{
    freemesh(&m_mesh);
    karin_GLWidget::deinit();
}

void karin_NLTViewer::paintGL()
{
    if(m_mesh.count == 0)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render2d(0, SCENE2D_RANGE, 0, SCENE2D_RANGE);
    {
        setmvp(0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        rendermesh(&m_mesh);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glFlush();
}

void karin_NLTViewer::open(const QString &qfile)
{
    material_s *mat;
    unsigned char *data;
    int channel;
    int width, height;
    NL_texture_s nl;
    const char *file;
    std::string std_file;

    std_file = qfile.toStdString();
    file = std_file.c_str();
    channel = 0;

    if(m_mesh.count == 0)
    {
        GLfloat vs[] = {
            0, 0,
            SCENE2D_RANGE, 0,
            0, SCENE2D_RANGE,

            0, SCENE2D_RANGE,
            SCENE2D_RANGE, 0,
            SCENE2D_RANGE, SCENE2D_RANGE
        };
        GLfloat ts[] = {
            0, 1,
            1, 1,
            0, 0,
            0, 0,
            1, 1,
            1, 0
        };
        newmesh(&m_mesh, 1);
        mat = m_mesh.materials;
        newmat(mat, 6);
        mat->use_color = true;

        for(int i = 0; i < 6; i++)
        {
            mat->points[i].vertex[0] = vs[i * 2];
            mat->points[i].vertex[1] = vs[i * 2 + 1];
            mat->points[i].vertex[2] = 0;
            mat->points[i].vertex[3] = 1;

            mat->points[i].texcoord[0] = ts[i * 2];
            mat->points[i].texcoord[1] = ts[i * 2 + 1];

            mat->points[i].normal[0] = 0;
            mat->points[i].normal[1] = 1;
            mat->points[i].normal[2] = 0;
        }
    }

    mat = m_mesh.materials;
    if(glIsTexture(mat->texture.tex_id))
    {
        glDeleteTextures(1, &mat->texture.tex_id);
        memset(&mat->texture, 0, sizeof(texture2d_s));
    }

    if(nlIsNLTextureFile(file) == 0)
    {
        memset(&nl, 0, sizeof(NL_texture_s));
        if(nlReadFile(&nl, file) != 0)
        {
            qDebug() << "[Error]: Load nlt file fail -> " << nlGetErrorString(nl_error);
            data = SOIL_load_image(file, &width, &height, &channel, SOIL_LOAD_AUTO);
        }
        else
        {
            nlMakeGLTexture(&data, &nl, &width, &height);
            nlFreeNLTexture(&nl);
        }
    }
    else
    {
        data = SOIL_load_image(file, &width, &height, &channel, SOIL_LOAD_AUTO);
    }

    if(data)
    {
        switch(channel)
        {
        case SOIL_LOAD_RGBA:
            mat->texture.format = GL_RGBA;
            break;
        case SOIL_LOAD_RGB:
            mat->texture.format = GL_RGB;
            break;
        case 0:
            break;
        default:
            goto __Exit;
            break;
        }
        mat->texture.width = width;
        mat->texture.height = height;
        loadtex2d(&mat->texture, data);
    }
    qDebug()<<mat->texture.tex_id<<mat->texture.width<<mat->texture.height<<channel<<qfile;


    updateGL();

__Exit:
    free(data);
}

void karin_NLTViewer::tonlt(const QString &dst, const QString &src)
{
    unsigned char *data;
    int channel;
    int width, height;
    NL_texture_s nl;
    const char *file;
    std::string std_file;

    std_file = src.toStdString();
    file = std_file.c_str();

    data = SOIL_load_image(file, &width, &height, &channel, SOIL_LOAD_AUTO);
    if(data && (channel == SOIL_LOAD_RGBA || channel == SOIL_LOAD_RGB))
    {
        if(nlMakeNLTexture(&nl, width, height, channel == SOIL_LOAD_RGBA ? 1 : 0, data) != NL_NO_ERROR)
        {
            qWarning() << "[Error]: Convert to nlt fail -> " << nlGetErrorString(nl_error);
        }
        else
        {
            if(nlWriteFile(&nl, dst.toStdString().c_str()) != NL_NO_ERROR)
                qWarning() << "[Error]: Write to nlt fail -> " << nlGetErrorString(nl_error);
        }
    }

    free(data);
}
