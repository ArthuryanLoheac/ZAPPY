import type {ReactNode} from 'react';
import clsx from 'clsx';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';
import HomepageFeatures from '@site/src/components/HomepageFeatures';
import Heading from '@theme/Heading';

import styles from './index.module.css';

function HomepageHeader() {
  const {siteConfig} = useDocusaurusContext();
  return (
    <header className={clsx('hero hero--primary', styles.heroBanner)}>
      <div className="container">
        <Heading as="h1" className="hero__title">
          {siteConfig.title}
        </Heading>
        <p className="hero__subtitle">{siteConfig.tagline}</p>
        <div className={styles.buttons}>
          <Link
            className="button button--secondary button--lg zoup-text"
            to="/docs/zappy/CONTRIBUTING">
            Zoup Zoup !
          </Link>
        </div>
      </div>
    </header>
  );
}

export default function Home(): ReactNode {
  const {siteConfig} = useDocusaurusContext();
  return (
    <Layout
      title={`Hello from ${siteConfig.title}`}
      description="Description will go into a meta tag in <head />">
      <HomepageHeader />
      <main>
        <div style={{justifyItems: 'center', padding: '5%'}}>
          <h1>Welcome to the <em>Zappytto</em> documentation !</h1>
          <p>This project is a school project which goal is to create a simulation of a world with AI trying to survive and level up to level 8</p>
          <p>
            This website contains the documentation of the project (protocols, API, contributions rules) <br />
            You can just take a look at the <Link href='https://github.com/ArthuryanLoheac/ZAPPY'>Github</Link> of the project to have more information about it !
          </p> <br />
          <p>
            The project is separated in three main parts:
            <ul>
              <li> The server, which is the core of the project, it handles the communication between the clients and the AI</li>
              <li> The AI, which is the brain of the project, it handles the logic of the robots</li>
              <li> The GUI, which is the interface of the project, it handles the display of the world and the robots</li>
            </ul>
          </p>

        </div>
      </main>
    </Layout>
  );
}
