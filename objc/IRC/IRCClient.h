// Created by Satoshi Nakagawa.
// You can redistribute it and/or modify it under the Ruby's license or the GPL2.

#import <Cocoa/Cocoa.h>
#import "IRCTreeItem.h"
#import "IRCClientConfig.h"
#import "IRCChannel.h"
#import "IRCConnection.h"
#import "IRCISupportInfo.h"
#import "IRCUserMode.h"
#import "Preferences.h"
#import "LogController.h"
#import "ServerDialog.h"
#import "Timer.h"
#import "HostResolver.h"


@class IRCWorld;


typedef enum {
	CONNECT_NORMAL,
	CONNECT_RECONNECT,
	CONNECT_RETRY,
} ConnectMode;


@interface IRCClient : IRCTreeItem
{
	IRCWorld* world;
	IRCClientConfig* config;
	
	NSMutableArray* channels;
	IRCISupportInfo* isupport;
	IRCUserMode* myMode;
	
	IRCConnection* conn;
	int connectDelay;
	BOOL reconnectEnabled;
	BOOL retryEnabled;
	
	BOOL isConnecting;
	BOOL isConnected;
	BOOL isReconnecting;
	BOOL isLoggedIn;
	BOOL isQuitting;
	NSStringEncoding encoding;
	
	NSString* inputNick;
	NSString* sentNick;
	NSString* myNick;
	int tryingNickNumber;
	
	NSString* serverHostname;
	BOOL inWhois;
	BOOL identifyMsg;
	BOOL identifyCTCP;
	
	AddressDetectionType addressDetectionMethod;
	HostResolver* nameResolver;
	NSString* joinMyAddress;
	NSString* myAddress;
	
	Timer* quitTimer;
	Timer* reconnectTimer;
	Timer* retryTimer;
	
	IRCChannel* lastSelectedChannel;
	
	NSMutableArray* whoisDialogs;
	ServerDialog* propertyDialog;
}

@property (nonatomic, assign) IRCWorld* world;
@property (nonatomic, readonly) NSString* name;
@property (nonatomic, readonly) IRCClientConfig* config;
@property (nonatomic, readonly) IRCISupportInfo* isupport;
@property (nonatomic, readonly) NSMutableArray* channels;
@property (nonatomic, readonly) BOOL isConnecting;
@property (nonatomic, readonly) BOOL isConnected;
@property (nonatomic, readonly) BOOL isReconnecting;
@property (nonatomic, readonly) BOOL isLoggedIn;
@property (nonatomic, readonly) NSString* myNick;
@property (nonatomic, readonly) NSString* myAddress;
@property (nonatomic, retain) IRCChannel* lastSelectedChannel;
@property (nonatomic, retain) ServerDialog* propertyDialog;

- (void)setup:(IRCClientConfig*)seed;
- (void)updateConfig:(IRCClientConfig*)seed;
- (IRCClientConfig*)storedConfig;
- (NSMutableDictionary*)dictionaryValue;

- (void)autoConnect:(int)delay;
- (void)onTimer;
- (void)terminate;
- (void)closeDialogs;
- (void)preferencesChanged;

- (void)connect;
- (void)connect:(ConnectMode)mode;
- (void)disconnect;
- (void)quit;
- (void)cancelReconnect;

- (void)changeNick:(NSString*)newNick;
- (void)joinChannel:(IRCChannel*)channel;
- (void)partChannel:(IRCChannel*)channel;
- (void)sendWhois:(NSString*)nick;
- (void)changeOp:(IRCChannel*)channel users:(NSArray*)users mode:(char)mode value:(BOOL)value;
- (void)kick:(IRCChannel*)channel target:(NSString*)nick;
- (void)sendFile:(NSString*)nick port:(int)port fileName:(NSString*)fileName size:(long long)size;
- (void)sendCTCPQuery:(NSString*)nick command:(NSString*)command text:(NSString*)text;

- (BOOL)inputText:(NSString*)s command:(NSString*)command;
- (void)sendText:(NSString*)s command:(NSString*)command channel:(IRCChannel*)channel;
- (void)sendLine:(NSString*)str;
- (void)send:(NSString*)str, ...;

- (IRCChannel*)findChannel:(NSString*)name;
- (int)indexOfTalkChannel;

@end
