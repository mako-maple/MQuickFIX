noinst_PROGRAMS = tradeclient

tradeclient_SOURCES = \
	tradeclient.cpp \
	Application.h \
	Application.cpp \
	TestRequest.cpp                \
	OrderMassCancelRequest.cpp     \
	SecurityListRequest.cpp        \
	NewOrderSingle.cpp             \
	OrderCancelRequest.cpp         \
	OrderCancelReplaceRequest.cpp  \
	OrderStatusRequest.cpp         \
	MarketDataRequest.cpp          \
	OrderMassStatusRequest.cpp     \
	RequestForPositions.cpp        \
	ExecutionReport.cpp              \
	OrderCancelReject.cpp            \
	TradingSessionStatus.cpp         \
	BusinessMessageReject.cpp        \
	OrderMassCancelReport.cpp        \
	SecurityList.cpp                 \
	SecurityList.h                   \
	Email.cpp                        \
	ListStatus.cpp                   \
	MarketDataIncrementalRefresh.cpp \
	MarketDataRequestReject.cpp      \
	RequestForPositionsAck.cpp       \
	PositionReport.cpp               \
	CollateralResponse.cpp           \
	CollateralReport.cpp             \
	OrderRateUpdate.h            \
	OrderRateUpdate.cpp          \
	PartyDetailsListReport.h     \
	PartyDetailsListReport.cpp   

tradeclient_LDADD = $(top_builddir)/src/C++/libquickfix.la 

INCLUDES = -I$(top_builddir)/include -I..

all-local:
	rm -f ../../bin/tradeclient
	ln -s ../examples/tradeclient/tradeclient ../../bin/tradeclient
