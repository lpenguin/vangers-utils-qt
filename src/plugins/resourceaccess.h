#ifndef RESOURCEACCESS_H
#define RESOURCEACCESS_H

#include <QFile>
#include <QIODevice>


namespace vangers {

template <typename TResource>
class AbstractResourceAccess {
public:
    virtual QSharedPointer<TResource> read(QIODevice& device) = 0;
    virtual void write(const QSharedPointer<TResource>& resource, QIODevice& device) = 0;
    virtual ~AbstractResourceAccess() {}
};

}


#endif // RESOURCEACCESS_H
