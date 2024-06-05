#ifndef STYLE_APPLICATION_UI_DOWNLOADER_HPP
#define STYLE_APPLICATION_UI_DOWNLOADER_HPP

#include <QObject>
#include "common/model_manager.hpp"

class QThread;

class Downloader : public QObject {
public:
    Downloader();

    void download(modelManager::PretrainedModel model);
private:
    std::shared_ptr<QThread> _thread;
};

#endif //STYLE_APPLICATION_UI_DOWNLOADER_HPP