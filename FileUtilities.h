#ifndef FILEUTILITIES_H
#define FILEUTILITIES_H

#include <algorithm>
#include <functional>
#include <vector>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>
#include <string>
#include <fstream>
using namespace std;

struct FileUtilities
{
    static QString addSeparator(QString st)
    {
        QString res = st.trimmed();
        if(res.length() == 0) return "";
        if(res[res.length() - 1] == '/')
            return res;
        else
            return res + '/';
    }

    static QStringList findAllDir(const QString dir)
    {
        QDir d(dir);
        d.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
        d.setSorting(QDir::Size | QDir::Reversed);

        QFileInfoList list = d.entryInfoList();
        QStringList sl;
        for(int i = 0; i < list.size(); i++)
        {
            if(list.at(i).fileName() == ".." ||
                    list.at(i).fileName() == ".")
                continue;
            sl.append(list.at(i).fileName());
        }
        return sl;
    }

    static QStringList findAllDir(const string dir)
    {
        return findAllDir(QString(dir.c_str()));
    }

    static QStringList findAllFile(const QString dir)
    {
        QDir d(dir);
        d.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        d.setSorting(QDir::Size | QDir::Reversed);

        QFileInfoList list = d.entryInfoList();
        QStringList sl;
        for(int i = 0; i < list.size(); i++)
        {
            if(list.at(i).fileName() == ".." ||
                    list.at(i).fileName() == ".")
                continue;
            sl.append(list.at(i).fileName());
        }
        return sl;
    }

    static QStringList findAllFile(const string dir)
    {
        return findAllFile(QString(dir.c_str()));
    }

    static QStringList findAllFileWithPath(const QString dir)
    {
        QDir d(dir);
        d.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        d.setSorting(QDir::Size | QDir::Reversed);

        QFileInfoList list = d.entryInfoList();
        QStringList sl;
        for(int i = 0; i < list.size(); i++)
        {
            if(list.at(i).fileName() == ".." ||
                    list.at(i).fileName() == ".")
                continue;
            sl.append(list.at(i).filePath());
        }
        return sl;
    }

    static string readToEnd(const char *path)
    {
        ifstream fin(path);
        string s, res;
        while(fin>>s)
            res += s;
        fin.close();
        return res;
    }

    static void writeToFile(const char *path, const string content)
    {
        ofstream fout(path);
        fout<<content;
        fout.close();
    }

    static char *home()
    {
        return getenv("HOME");
    }

    static QString qhome()
    {
        return QString(home());
    }

    static bool copyDir(const QDir &_fromDir, const QDir &_toDir, bool bCoverIfFileExists)
    {
        QDir fromDir = _fromDir;
        QDir toDir = _toDir;
        if(!toDir.exists())
            if(!toDir.mkdir(toDir.path()))
                return false;
        QFileInfoList fileInfoList = fromDir.entryInfoList();
        foreach(QFileInfo fileInfo, fileInfoList)
        {
            if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
                continue;
            if(fileInfo.isDir())
            {
                if(!copyDir(fileInfo.filePath(), toDir.filePath(fileInfo.fileName()), bCoverIfFileExists))
                    return false;
            }
            else
            {
                if(bCoverIfFileExists && toDir.exists(fileInfo.fileName()))
                    toDir.remove(fileInfo.fileName());
                if(!QFile::copy(fileInfo.filePath(), toDir.filePath(fileInfo.fileName())))
                    return false;
            }
        }
        return true;
    }

    static bool deleteDir(const QString &dirName)
    {
        QDir dir(dirName);
        if(!dir.exists()) return true;
        QStringList files = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
        QStringList::iterator f = files.begin();
        bool error = false;
        for(; f != files.end(); f++)
        {
            QString filePath = dir.path() + '/' + *f;
            QFileInfo fi(filePath);
            if(fi.isFile() || fi.isSymLink())
            {
                QFile::setPermissions(filePath, QFile::WriteOwner);
                if(!QFile::remove(filePath))
                {
                    qDebug() << "failed to remove file: " << filePath;
                    error = true;
                }
            }
            else if(fi.isDir())
            {
                if(!deleteDir(filePath))
                {
                    error = true;
                }
            }
        }
        if(!dir.rmdir(dir.path()))
        {
            qDebug() << "failed to remove dir: " << dir.path();
            error = true;
        }
        return !error;
    }

};

#endif // FILEUTILITIES_H
