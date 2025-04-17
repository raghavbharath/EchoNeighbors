import { Tabs } from 'expo-router';
import { useState, useEffect, useRef } from "react";
import React from 'react';
import { Platform, StyleSheet, Text, View } from 'react-native';

import { HapticTab } from '@/components/HapticTab';
import TabBarBackground from '@/components/ui/TabBarBackground';
import { Colors } from '@/constants/Colors';
import { useColorScheme } from '@/hooks/useColorScheme';
import AntDesign from '@expo/vector-icons/AntDesign';
import Ionicons from '@expo/vector-icons/Ionicons';
import { usePushNotifications } from "../../hooks/usePushNotifications";
import { StatusBar } from "expo-status-bar";

export default function TabLayout() {
  const colorScheme = useColorScheme();
  const { expoPushToken, notification } = usePushNotifications();

  // Log the token in the console
  console.log("Expo Push Token:", expoPushToken);

  useEffect(() => {
    if (expoPushToken) {
      console.log("📡 Sending token to backend:", expoPushToken);
  
      fetch("https://b0ab-128-235-85-14.ngrok-free.app/register-token", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          token: expoPushToken,
          user: "neighbor1",
        }),
      })
        .then((res) => {
          console.log("🌐 Got response from server:", res.status);
          return res.json();
        })
        .then((data) => {
          console.log("✅ Token registered:", data);
        })
        .catch((err) => {
          console.error("❌ Failed to register token:", err.message || err);
        });
    }
  }, [expoPushToken]);

  return (
    <>

      <Tabs
        screenOptions={{
          tabBarActiveTintColor: Colors[colorScheme ?? 'light'].tint,
          headerShown: false,
          tabBarButton: HapticTab,
          tabBarBackground: TabBarBackground,
          tabBarStyle: Platform.select({
            ios: {
              // Use a transparent background on iOS to show the blur effect
              position: 'absolute',
            },
            default: {},
          }),
        }}>
        <Tabs.Screen
          name="index"
          options={{
            title: 'Map',
            tabBarIcon: ({ color }) => <AntDesign name="enviroment" size={24} color="white" />,
          }}
        />
        <Tabs.Screen
          name="explore"
          options={{
            title: 'Notifications',
            tabBarIcon: ({ color }) => <Ionicons name="notifications" size={24} color="white" />,
          }}
        />
      </Tabs>
    </>
  );
}

const styles = StyleSheet.create({
  tokenContainer: {
    position: 'absolute',
    top: 60,
    left: 20,
    right: 20,
    zIndex: 999,
    backgroundColor: '#f0f0f0',
    padding: 12,
    borderRadius: 8,
    shadowColor: '#000',
    shadowOpacity: 0.1,
    shadowOffset: { width: 0, height: 2 },
    shadowRadius: 4,
    elevation: 3,
  },
  tokenText: {
    fontSize: 14,
    fontWeight: 'bold',
    marginBottom: 4,
  },
  tokenValue: {
    fontSize: 12,
    color: '#333',
  },
});