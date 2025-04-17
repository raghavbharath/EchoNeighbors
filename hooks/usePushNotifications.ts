import { useState, useEffect, useRef } from "react";
import * as Device from "expo-device";
import * as Notifications from "expo-notifications";
import { Platform } from "react-native";

// Set how incoming notifications behave
Notifications.setNotificationHandler({
  handleNotification: async () => ({
    shouldPlaySound: false,
    shouldShowAlert: true,
    shouldSetBadge: false,
  }),
});

// Define the return type
export interface PushNotificationState {
  expoPushToken?: string;
  notification?: Notifications.Notification;
}

export const usePushNotifications = (): PushNotificationState => {
  const [expoPushToken, setExpoPushToken] = useState<string | undefined>();
  const [notification, setNotification] = useState<Notifications.Notification | undefined>();

  const notificationListener = useRef<Notifications.Subscription>();
  const responseListener = useRef<Notifications.Subscription>();

  useEffect(() => {
    // Register for push notifications
    registerForPushNotificationsAsync().then((token) => {
      if (token) {
        setExpoPushToken(token); // ✅ Save token in state
        console.log("Expo Push Token:", token); // ✅ Print token to console
      } else {
        console.warn("No token was returned.");
      }
    });

    // Listener: Notification received while app is foregrounded
    notificationListener.current = Notifications.addNotificationReceivedListener((notification) => {
      setNotification(notification);
    });

    // Listener: User interacted with a notification
    responseListener.current = Notifications.addNotificationResponseReceivedListener((response) => {
      console.log("Notification response:", response);
    });

    // Cleanup on unmount
    return () => {
      if (notificationListener.current) {
        Notifications.removeNotificationSubscription(notificationListener.current);
      }
      if (responseListener.current) {
        Notifications.removeNotificationSubscription(responseListener.current);
      }
    };
  }, []);

  return {
    expoPushToken,
    notification,
  };
};

// Async function to get permissions and the push token
async function registerForPushNotificationsAsync(): Promise<string | undefined> {
  try {
    if (!Device.isDevice || Platform.OS === "web") {
      alert("Must use physical device (not web) for push notifications");
      console.warn("Not a physical device. Device.isDevice =", Device.isDevice, " Platform =", Platform.OS);
      return;
    }

    const { status: existingStatus } = await Notifications.getPermissionsAsync();
    let finalStatus = existingStatus;

    if (existingStatus !== "granted") {
      const { status } = await Notifications.requestPermissionsAsync();
      finalStatus = status;
    }

    if (finalStatus !== "granted") {
      alert("Push notification permissions not granted");
      console.warn("Final notification status not granted:", finalStatus);
      return;
    }

    const tokenData = await Notifications.getExpoPushTokenAsync();
    console.log("Push token object returned:", tokenData);

    if (Platform.OS === "android") {
      await Notifications.setNotificationChannelAsync("default", {
        name: "default",
        importance: Notifications.AndroidImportance.MAX,
        vibrationPattern: [0, 250, 250, 250],
        lightColor: "#FF231F7C",
      });
    }

    return tokenData.data;
  } catch (error) {
    console.error("Error getting push token:", error);
    return;
  }
}