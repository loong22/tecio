/**
 * JTG 2016.01.25
 * moc with Qt4 chokes on certain boost 1.55+ macros
 * The solution is to prevent the problem headers from
 * being included during the moc processing of the source
 * file since they are not needed until compile time.
 */
#if defined Q_MOC_RUN
#  include <QtGlobal>
#  if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#    error "This issue has been fixed in Qt5. There is no need to wrap the header with the Q_MOC_RUN guard. This whole module should be removed."
#  endif
#else
#  include "ThirdPartyHeadersBegin.h"
#    include <boost/signals2.hpp>
#  include "ThirdPartyHeadersEnd.h"
#endif
