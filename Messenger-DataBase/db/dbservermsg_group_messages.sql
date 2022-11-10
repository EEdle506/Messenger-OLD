CREATE DATABASE  IF NOT EXISTS `dbservermsg` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `dbservermsg`;
-- MySQL dump 10.13  Distrib 8.0.29, for Win64 (x86_64)
--
-- Host: localhost    Database: dbservermsg
-- ------------------------------------------------------
-- Server version	8.0.29

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `group_messages`
--

DROP TABLE IF EXISTS `group_messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `group_messages` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `participantID` int NOT NULL,
  `msg` varchar(2048) NOT NULL,
  `msgDate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`),
  KEY `FK_GROUPMSG_groupID_idx` (`participantID`),
  CONSTRAINT `FK_GROUPMSG_groupID` FOREIGN KEY (`participantID`) REFERENCES `group_participant` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=46 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `group_messages`
--

LOCK TABLES `group_messages` WRITE;
/*!40000 ALTER TABLE `group_messages` DISABLE KEYS */;
INSERT INTO `group_messages` VALUES (25,78,'Hi bro','2022-05-30 15:55:54'),(26,78,'Hi bro','2022-05-30 17:19:44'),(27,78,'dahfgkhajdgfhkadkhfda','2022-05-30 17:19:54'),(28,78,'hi bro','2022-05-30 17:20:45'),(29,78,'dadada','2022-05-30 17:20:53'),(30,75,'Привіт','2022-05-30 17:27:48'),(31,75,'Хей бро, офвраорфвоафвдо фвоаофв аофв','2022-05-30 17:28:24'),(32,75,'фвлдраодфв','2022-05-30 17:32:10'),(33,75,'adljfhjaldhjlfad','2022-05-30 17:40:30'),(34,75,'lol? hi','2022-05-30 17:40:42'),(35,75,'hi bro','2022-05-30 17:42:57'),(36,75,'jfahdjfhjadhjlfajldfja','2022-05-30 17:44:15'),(37,75,'Lol, kek','2022-05-30 17:44:27'),(38,75,'text','2022-05-30 18:02:30'),(39,75,'fadfadfad','2022-05-30 18:05:12'),(40,75,'dadsadas','2022-05-30 18:07:31'),(41,75,'dafadfadfa','2022-05-30 18:09:55'),(42,75,'adfadfadjf jadjlfh jlad adjhdhjyssyfrhbugbh dbh bhg bh gjbh bh jkgbhgjk bhgj kybhgjky ghjkbyv ','2022-05-30 18:10:07'),(43,75,'hs wbjhk d sbjh sdedserasderv fdrtedefrtv dfretbdrtfefttfder','2022-05-30 18:10:30'),(44,75,'sedbvtfrjnhg kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkuijvjfrndtjuein bvijuder ijntdsfreijukbvndijukrnbvf ikjduftrni krjnbivkdfjtunvijudrtnsvijudrtfnivjugdsrtfo','2022-05-30 18:10:43'),(45,78,'asddeqawsedadesdewrswde','2022-05-30 18:13:16');
/*!40000 ALTER TABLE `group_messages` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-05-31  1:44:14
